#include "graphics.h"
#include "ui/ui.h"
#include "render_chain.h"
#include <RBX/task_scheduler/task_scheduler.hpp>
#include <RBX/environment/libs/drawingimmediate/drawingimmediate.hpp>
#include <Utilitys/Log.hpp>

#include <imgui/addons/imgui_notify.h>


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

std::unique_ptr<module::core::graphics::c_graphics> graphics = std::make_unique<module::core::graphics::c_graphics>();

float module::core::graphics::c_graphics::dpi_scale = 1.0f;
bool module::core::graphics::c_graphics::show_ui = false;

namespace
{
    template<typename T>
    void release_probe_reference(T*& object)
    {
        T* owned = object;
        object = nullptr;
        if (!owned) return;
        __try { owned->Release(); }
        __except (EXCEPTION_EXECUTE_HANDLER) {}
    }

    bool acquire_render_device(IDXGISwapChain* swapchain, DXGI_SWAP_CHAIN_DESC& desc,
        ID3D11Device*& device, ID3D11DeviceContext*& context)
    {
        device = nullptr;
        context = nullptr;
        desc = {};
        bool acquired = false;
        // Keep SEH in this leaf helper; initialization owns C++ objects.
        __try
        {
            if (SUCCEEDED(swapchain->GetDesc(&desc)) && IsWindow(desc.OutputWindow) &&
                SUCCEEDED(swapchain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&device))) && device)
            {
                device->GetImmediateContext(&context);
                acquired = context != nullptr;
            }
        }
        __except (EXCEPTION_EXECUTE_HANDLER) { acquired = false; }
        if (!acquired)
        {
            release_probe_reference(context);
            release_probe_reference(device);
        }
        return acquired;
    }
}

ID3D11Device* module::core::graphics::c_graphics::get_device()
{
    return graphics->d3d11_device;
}

void module::core::graphics::c_graphics::initialize()
{
    if (is_initialized)
        return;

    detail::SwapChainCandidate candidate{};
    DXGI_SWAP_CHAIN_DESC desc{};
    ID3D11Device* acquired_device = nullptr;
    ID3D11DeviceContext* acquired_context = nullptr;
    bool ready = false;
    constexpr size_t maximum_attempts = 300;
    for (size_t attempt = 0; attempt < maximum_attempts; ++attempt)
    {
        const uintptr_t render_job = task_scheduler::GetJobByTypeName("RenderJob");
        if (detail::resolve_swapchain(render_job, candidate) &&
            acquire_render_device(candidate.swapchain, desc, acquired_device, acquired_context))
        {
            ready = true;
            break;
        }
        Sleep(100);
    }
    if (!ready)
    {
        LOGF("Graphics", "RenderJob swapchain was not ready after 30 seconds; initialization skipped");
        return;
    }
    swapchain = candidate.swapchain;
    window_handle = desc.OutputWindow;
    d3d11_device = acquired_device;
    d3d11_devicecontext = acquired_context;

    void** orig_vtable = candidate.vtable;
    constexpr size_t vtable_size = 18;

    static void** shadow_vtable = nullptr;
    if (!shadow_vtable)
    {
        shadow_vtable = new void* [vtable_size];
        memcpy(shadow_vtable, orig_vtable, sizeof(void*) * vtable_size);
    }

    org_present = reinterpret_cast<present_t>(orig_vtable[8]);
    shadow_vtable[8] = reinterpret_cast<void*>(&hk_present);

    org_resize_buffer = reinterpret_cast<resize_buffer_t>(orig_vtable[13]);
    shadow_vtable[13] = reinterpret_cast<void*>(&hk_resize_buffers);

    *reinterpret_cast<void***>(swapchain) = shadow_vtable;

    org_window_proc = reinterpret_cast<WNDPROC>(
        SetWindowLongPtrW(window_handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&hk_wndproc)));

    is_initialized = true;
    LOGF("Graphics", "Renderer initialized with the current RenderView offsets");
}

bool module::core::graphics::c_graphics::initialize_imgui()
{
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingWithShift = false;
    io.ConfigDockingTransparentPayload = true;
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;

    HDC hdc = GetDC(window_handle);
    int dpi = GetDeviceCaps(hdc, LOGPIXELSX);
    ReleaseDC(window_handle, hdc);
    dpi_scale = dpi / 96.0f;

    if (!d3d11_rendertargetview)
    {
        ID3D11Texture2D* bb = nullptr;
        if (SUCCEEDED(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&bb))))
        {
            d3d11_device->CreateRenderTargetView(bb, nullptr, &d3d11_rendertargetview);
            bb->Release();
        }
        else return false;
    }

    ImGui_ImplWin32_Init(window_handle);
    ImGui_ImplDX11_Init(d3d11_device, d3d11_devicecontext);

    // Real's menu draws with Segoe UI and merges Segoe UI Emoji over it, which
    // is what gives its labels the coloured icons; the editor uses Consolas.
    // Everything is loaded defensively so a machine without a font still runs
    // with the built-in bitmap font instead of asserting.
    static auto font_exists = [](const char* path) -> bool
    {
        const DWORD attributes = GetFileAttributesA(path);
        return attributes != INVALID_FILE_ATTRIBUTES;
    };

    static const ImWchar ui_ranges[] = {
        0x0020, 0x00FF, // basic latin + latin supplement
        0x0100, 0x024F, // latin extended
        0x0370, 0x03FF, // greek
        0x0400, 0x04FF, // cyrillic
        0x2000, 0x206F, // general punctuation
        0x20A0, 0x20CF, // currency
        0x2190, 0x21FF, // arrows
        0x2200, 0x22FF, // math
        0x2500, 0x257F, // box drawing
        0,
    };
    static const ImWchar icon_ranges[] = {
        0xE700, 0xEC00, // Segoe MDL2 Assets private-use icon block
        0,
    };

    const float font_size = 15.0f * dpi_scale;

    ImFontConfig main_cfg;
    main_cfg.OversampleH = 2;
    main_cfg.OversampleV = 1;
    main_cfg.PixelSnapH = true;

    ImFont* ui_font = nullptr;
    if (font_exists("C:\\Windows\\Fonts\\segoeui.ttf"))
        ui_font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", font_size, &main_cfg, ui_ranges);

    if (!ui_font)
        io.Fonts->AddFontDefault();

    if (font_exists("C:\\Windows\\Fonts\\segmdl2.ttf"))
    {
        ImFontConfig icon_cfg;
        icon_cfg.MergeMode = true;
        icon_cfg.PixelSnapH = true;
        io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segmdl2.ttf", font_size, &icon_cfg, icon_ranges);
    }

    if (font_exists("C:\\Windows\\Fonts\\consola.ttf"))
    {
        ImFontConfig mono_cfg;
        mono_cfg.OversampleH = 2;
        mono_cfg.PixelSnapH = true;
        io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", 14.0f * dpi_scale, &mono_cfg, ui_ranges);
    }

    io.Fonts->Build();

    ImGuiStyle& style = ImGui::GetStyle();

    // Neutral #121212 surfaces, hairline borders and small radii. Interactive
    // elements are white on dark instead of a coloured accent, which keeps the
    // menu reading like a desktop tool rather than a template.
    style.WindowPadding = ImVec2(16.0f, 12.0f);
    style.FramePadding = ImVec2(10.0f, 5.0f);
    style.CellPadding = ImVec2(8.0f, 4.0f);
    style.ItemSpacing = ImVec2(10.0f, 8.0f);
    style.ItemInnerSpacing = ImVec2(8.0f, 6.0f);
    style.IndentSpacing = 16.0f;
    style.ScrollbarSize = 9.0f;
    style.GrabMinSize = 9.0f;

    style.WindowRounding = 6.0f;
    style.ChildRounding = 5.0f;
    style.FrameRounding = 4.0f;
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 6.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;

    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;

    style.Alpha = 1.0f;
    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;

    style.WindowTitleAlign = ImVec2(0.0f, 0.5f);

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.91f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.52f, 0.53f, 0.55f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.071f, 0.071f, 0.071f, 1.00f);   // #121212
    colors[ImGuiCol_ChildBg] = ImVec4(0.078f, 0.078f, 0.078f, 1.00f);    // #141414
    colors[ImGuiCol_PopupBg] = ImVec4(0.086f, 0.086f, 0.086f, 1.00f);    // #161616
    colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.08f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.045f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.075f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.11f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.071f, 0.071f, 0.071f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.071f, 0.071f, 0.071f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.071f, 0.071f, 0.071f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.078f, 0.078f, 0.078f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.12f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.28f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.92f, 0.92f, 0.93f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.82f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.95f, 0.95f, 0.96f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(1.00f, 1.00f, 1.00f, 0.055f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.11f);
    colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.16f);
    colors[ImGuiCol_Header] = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.11f);
    colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.15f);
    colors[ImGuiCol_Separator] = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.16f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.24f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.12f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
    colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.08f);
    colors[ImGuiCol_TabActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.85f, 0.85f, 0.86f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.75f, 0.75f, 0.77f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.92f, 0.92f, 0.93f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(1.00f, 1.00f, 1.00f, 0.12f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.03f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.16f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.95f, 0.95f, 0.96f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.24f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.40f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.55f);

    style.ScaleAllSizes(dpi_scale);

    // Upload the Hugeicons atlas once. Every icon in the menu comes from this
    // single immutable texture, so the UI no longer depends on which icon font
    // happens to be installed on the machine.
    {
        D3D11_TEXTURE2D_DESC desc{};
        desc.Width = (UINT)ui::icon_atlas_width;
        desc.Height = (UINT)ui::icon_atlas_height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        D3D11_SUBRESOURCE_DATA data{};
        data.pSysMem = ui::icon_atlas_rgba;
        data.SysMemPitch = (UINT)ui::icon_atlas_width * 4;

        ID3D11Texture2D* texture = nullptr;
        if (SUCCEEDED(d3d11_device->CreateTexture2D(&desc, &data, &texture)))
        {
            ID3D11ShaderResourceView* view = nullptr;
            if (SUCCEEDED(d3d11_device->CreateShaderResourceView(texture, nullptr, &view)))
                ui::c_ui::icon_atlas = view;
            texture->Release();
        }
    }

    ui::c_ui::initialize();

    return true;
}

HRESULT WINAPI module::core::graphics::c_graphics::hk_present(IDXGISwapChain* swapchain, UINT sync, UINT flags)
{
    static std::once_flag imgui_init;
    std::call_once(imgui_init, []() { graphics->is_imgui_initialized = graphics->initialize_imgui(); });
    if (!graphics->is_imgui_initialized)
        return graphics->org_present(swapchain, sync, flags);

    if (!graphics->d3d11_rendertargetview)
    {
        ID3D11Texture2D* bb = nullptr;
        if (SUCCEEDED(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&bb))))
        {
            graphics->d3d11_device->CreateRenderTargetView(bb, nullptr, &graphics->d3d11_rendertargetview);
            bb->Release();
        }
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    DrawingImmediate::RenderOverlay();

    if (show_ui)
        ui::c_ui::draw();

    ImGui::RenderNotifications();

    ImGui::Render();
    graphics->d3d11_devicecontext->OMSetRenderTargets(1, &graphics->d3d11_rendertargetview, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return graphics->org_present(swapchain, sync, flags);
}

HRESULT WINAPI module::core::graphics::c_graphics::hk_resize_buffers(IDXGISwapChain* swapchain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT fmt, UINT flags)
{
    graphics->safe_release(graphics->d3d11_rendertargetview);
    return graphics->org_resize_buffer(swapchain, buffer_count, width, height, fmt, flags);
}

LRESULT CALLBACK module::core::graphics::c_graphics::hk_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (msg == WM_KEYDOWN)
    {
        if (wparam == VK_INSERT || wparam == VK_END || wparam == VK_F2)
            show_ui = !show_ui;
    }

    if (msg == WM_DPICHANGED)
    {
        dpi_scale = LOWORD(wparam) / 96.0f;
    }

    if (show_ui && ImGui::GetCurrentContext() && ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
        return true;

    switch (msg)
    {
    case 522: case 513: case 533: case 514: case 134:
    case 516: case 517: case 258: case 257: case 132:
    case 127: case 255: case 523: case 524: case 793:
        if (show_ui) return true;
        break;
    }

    return CallWindowProc(graphics->org_window_proc, hwnd, msg, wparam, lparam);
}
