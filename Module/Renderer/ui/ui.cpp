#include "ui.h"
#include "../graphics.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <chrono>
#include <cstdio>

#include <globals.h>
#include <managers/logging/logging.h>
#include <RBX/task_scheduler/task_scheduler.hpp>

std::deque<module::core::graphics::ui::ui_instruction_t> module::core::graphics::ui::ui_queue;

static std::unordered_map<std::string, bool> checkbox_states;
static std::unordered_map<std::string, float> slider_states;
static std::unordered_map<std::string, int> combo_states;
static std::unordered_map<std::string, int> listbox_states;

std::vector<module::core::graphics::ui::editor_tab_t> module::core::graphics::ui::c_ui::editor_tabs;
int module::core::graphics::ui::c_ui::active_tab = 0;
int module::core::graphics::ui::c_ui::tab_counter = 0;
int module::core::graphics::ui::c_ui::current_script = 0;

static const char* script_labels[] = {
    (("InfiniteYield")), (("UNC")), (("Vulnerability Test")), (("Unnamed-ESP")), (("Myriad")), (("Sunc")), (("Cobalt")), (("Dex"))
};

static const char* script_sources[] = {
    ("loadstring(game:HttpGet(\"https://raw.githubusercontent.com/EdgeIY/infiniteyield/master/source\"))()"),
    ("loadstring(game:HttpGet(\"https://raw.githubusercontent.com/unified-naming-convention/NamingStandard/refs/heads/main/UNCCheckEnv.lua\"))()"),
    ("loadstring(game:HttpGet(\"https://raw.githubusercontent.com/zryr/Vulnerability-Check/refs/heads/main/Script\"))()"),
    ("pcall(function() loadstring(game:HttpGet('https://raw.githubusercontent.com/ic3w0lf22/Unnamed-ESP/master/UnnamedESP.lua'))() end)"),
    ("loadstring(game:HttpGetAsync(\"https://github.com/Proton-Utilities/Myriad/releases/latest/download/myriad.luau\"))()"),
    ("getgenv().sUNCDebug = {[\"printcheckpoints\"] = false, [\"delaybetweentests\"] = 0, [\"printtesttimetaken\"] = false} loadstring(game:HttpGet(\"https://script.sunc.su/\"))()"),
    ("loadstring(game:HttpGet(\"https://github.com/notpoiu/cobalt/releases/latest/download/Cobalt.luau\"))()"),
    ("loadstring(game:HttpGet(\"https://raw.githubusercontent.com/infyiff/backup/main/dex.lua\"))()")
};

void module::core::graphics::ui::c_ui::fire_callback_void(int ref)
{
    if (ref == LUA_NOREF || ref == LUA_REFNIL) return;
    lua_State* L = globals.exploit_thread;
    if (!L) return;
    lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
    if (!lua_isfunction(L, -1)) { lua_pop(L, 1); return; }
    lua_pcall(L, 0, 0, 0);
}

void module::core::graphics::ui::c_ui::fire_callback_bool(int ref, bool val)
{
    if (ref == LUA_NOREF || ref == LUA_REFNIL) return;
    lua_State* L = globals.exploit_thread;
    if (!L) return;
    lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
    if (!lua_isfunction(L, -1)) { lua_pop(L, 1); return; }
    lua_pushboolean(L, val);
    lua_pcall(L, 1, 0, 0);
}

void module::core::graphics::ui::c_ui::fire_callback_float(int ref, float val)
{
    if (ref == LUA_NOREF || ref == LUA_REFNIL) return;
    lua_State* L = globals.exploit_thread;
    if (!L) return;
    lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
    if (!lua_isfunction(L, -1)) { lua_pop(L, 1); return; }
    lua_pushnumber(L, val);
    lua_pcall(L, 1, 0, 0);
}

void module::core::graphics::ui::c_ui::fire_callback_str(int ref, const std::string& val)
{
    if (ref == LUA_NOREF || ref == LUA_REFNIL) return;
    lua_State* L = globals.exploit_thread;
    if (!L) return;
    lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
    if (!lua_isfunction(L, -1)) { lua_pop(L, 1); return; }
    lua_pushstring(L, val.c_str());
    lua_pcall(L, 1, 0, 0);
}

void module::core::graphics::ui::c_ui::fire_callback_int(int ref, int val)
{
    if (ref == LUA_NOREF || ref == LUA_REFNIL) return;
    lua_State* L = globals.exploit_thread;
    if (!L) return;
    lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
    if (!lua_isfunction(L, -1)) { lua_pop(L, 1); return; }
    lua_pushinteger(L, val);
    lua_pcall(L, 1, 0, 0);
}

void module::core::graphics::ui::c_ui::fire_callback_color(int ref, float r, float g, float b, float a)
{
    if (ref == LUA_NOREF || ref == LUA_REFNIL) return;
    lua_State* L = globals.exploit_thread;
    if (!L) return;
    lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
    if (!lua_isfunction(L, -1)) { lua_pop(L, 1); return; }
    lua_pushnumber(L, r);
    lua_pushnumber(L, g);
    lua_pushnumber(L, b);
    lua_pushnumber(L, a);
    lua_pcall(L, 4, 0, 0);
}

void module::core::graphics::ui::c_ui::add_tab(const std::string& name, const std::string& content)
{
    tab_counter++;
    std::string tab_name = name.empty() ? (("tab ") + std::to_string(tab_counter)) : name;
    editor_tabs.emplace_back(tab_name);
    if (!content.empty())
        editor_tabs.back().editor.SetText(content);
    active_tab = static_cast<int>(editor_tabs.size()) - 1;
}

void module::core::graphics::ui::c_ui::close_tab(int index)
{
    if (index < 0 || index >= (int)editor_tabs.size()) return;
    if (editor_tabs.size() <= 1) return;
    editor_tabs.erase(editor_tabs.begin() + index);
    if (active_tab >= (int)editor_tabs.size())
        active_tab = (int)editor_tabs.size() - 1;
}

void module::core::graphics::ui::c_ui::initialize()
{
    add_tab(("tab 1"), ("print('updated')"));
}

static void execute_script(const std::string& src)
{
    if (src.empty()) return;
    task_scheduler::RequestExecution(src);
}

static void render_instruction(module::core::graphics::ui::ui_instruction_t* inst, const std::string& window_name)
{
    using namespace ImGui;
    switch (inst->type)
    {
    case module::core::graphics::ui::ui_instruction_t::TEXT:
    {
        if (!inst->text_title.empty() && !inst->text_content.empty())
            Text((("%s %s")), inst->text_title.c_str(), inst->text_content.c_str());
        else if (!inst->text_content.empty())
            Text((("%s")), inst->text_content.c_str());
        else
            Text((("%s")), inst->text_title.c_str());
        break;
    }
    case module::core::graphics::ui::ui_instruction_t::BUTTON:
    {
        if (Button(inst->button_title.c_str()))
            module::core::graphics::ui::c_ui::fire_callback_void(inst->button_callback);
        break;
    }
    case module::core::graphics::ui::ui_instruction_t::CHECKBOX:
    {
        std::string id = window_name + ("/") + inst->checkbox_title;
        bool& s = checkbox_states[id];
        if (Checkbox(inst->checkbox_title.c_str(), &s))
            module::core::graphics::ui::c_ui::fire_callback_bool(inst->checkbox_callback, s);
        break;
    }
    case module::core::graphics::ui::ui_instruction_t::INPUT:
    {
        if (!inst->input_title.empty())
            Text((("%s")), inst->input_title.c_str());
        char buf[512];
        strncpy_s(buf, inst->input_text.c_str(), sizeof(buf) - 1);
        std::string label = (("##input") + std::to_string(reinterpret_cast<uintptr_t>(inst)));
        if (InputText(label.c_str(), buf, sizeof(buf), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            inst->input_text = buf;
            module::core::graphics::ui::c_ui::fire_callback_str(inst->input_callback, inst->input_text);
        }
        break;
    }
    case module::core::graphics::ui::ui_instruction_t::SLIDER:
    {
        std::string id = window_name + ("/") + inst->slider_title;
        float& v = slider_states[id];
        if (SliderFloat(inst->slider_title.c_str(), &v, inst->slider_min, inst->slider_max))
            module::core::graphics::ui::c_ui::fire_callback_float(inst->slider_callback, v);
        break;
    }
    case module::core::graphics::ui::ui_instruction_t::LISTBOX:
    {
        std::string id = window_name + ("/") + inst->listbox_title;
        if (!listbox_states.count(id)) listbox_states[id] = inst->current_item - 1;
        int& sel = listbox_states[id];
        if (BeginListBox(inst->listbox_title.c_str(), ImVec2(200, 100)))
        {
            for (int i = 0; i < (int)inst->items.size(); i++)
            {
                if (Selectable(inst->items[i].c_str(), sel == i))
                {
                    sel = i;
                    module::core::graphics::ui::c_ui::fire_callback_int(inst->listbox_callback, sel + 1);
                }
                if (sel == i) SetItemDefaultFocus();
            }
            EndListBox();
        }
        break;
    }
    case module::core::graphics::ui::ui_instruction_t::COMBO:
    {
        std::string id = window_name + ("/") + inst->combo_title;
        if (!combo_states.count(id)) combo_states[id] = inst->current_item - 1;
        int& sel = combo_states[id];
        std::vector<const char*> items_c;
        for (auto& item : inst->items)
            items_c.push_back(item.c_str());
        if (!items_c.empty())
        {
            if (Combo(inst->combo_title.c_str(), &sel, items_c.data(), (int)items_c.size()))
                module::core::graphics::ui::c_ui::fire_callback_int(inst->combo_callback, sel + 1);
        }
        break;
    }
    case module::core::graphics::ui::ui_instruction_t::COLOR_PICKER:
    {
        if (ColorEdit4(inst->text_title.c_str(), inst->color))
            module::core::graphics::ui::c_ui::fire_callback_color(inst->color_callback, inst->color[0], inst->color[1], inst->color[2], inst->color[3]);
        break;
    }
    case module::core::graphics::ui::ui_instruction_t::PROGRESS_BAR:
    {
        std::string overlay = std::to_string((int)(inst->progress_value * 100)) + ("%");
        ProgressBar(inst->progress_value, ImVec2(-1, 0), overlay.c_str());
        break;
    }
    case module::core::graphics::ui::ui_instruction_t::SEPARATOR: { Separator(); break; }
    case module::core::graphics::ui::ui_instruction_t::SAME_LINE: { SameLine(); break; }
    case module::core::graphics::ui::ui_instruction_t::SPACING: { Spacing(); break; }
    case module::core::graphics::ui::ui_instruction_t::TREE_NODE:
    {
        if (TreeNode(inst->treename_title.c_str()))
        {
            module::core::graphics::ui::c_ui::fire_callback_void(inst->treenode_callback);
            TreePop();
        }
        break;
    }
    case module::core::graphics::ui::ui_instruction_t::TOOLTIP:
    {
        if (IsItemHovered())
        {
            BeginTooltip();
            Text(("%s"), inst->tooltip_text.c_str());
            EndTooltip();
        }
        break;
    }
    case module::core::graphics::ui::ui_instruction_t::COLUMNS: { Columns(inst->columns_count); break; }
    case module::core::graphics::ui::ui_instruction_t::NEXT_COLUMN: { NextColumn(); break; }
    default: break;
    }
}

void module::core::graphics::ui::c_ui::draw_lua_ui()
{
    using namespace ImGui;
    std::unordered_map<std::string, std::vector<module::core::graphics::ui::ui_instruction_t*>> window_map;
    for (auto& inst : ui_queue)
        window_map[inst.ui_title].push_back(&inst);
    for (auto& [window_name, instructions] : window_map)
    {
        if (!Begin(window_name.c_str())) { End(); continue; }
        bool has_tabbar = false;
        std::string tabbar_name;
        for (auto* inst : instructions)
        {
            if (inst->type == module::core::graphics::ui::ui_instruction_t::TABBAR)
            {
                has_tabbar = true;
                tabbar_name = inst->tabbar_title;
                break;
            }
        }
        if (has_tabbar && BeginTabBar(tabbar_name.c_str()))
        {
            std::unordered_map<std::string, std::vector<module::core::graphics::ui::ui_instruction_t*>> tab_content;
            for (auto* inst : instructions)
            {
                if (!inst->parent_tabbar.empty())
                    tab_content[inst->parent_tabbar].push_back(inst);
            }
            for (auto* inst : instructions)
            {
                if (inst->type == module::core::graphics::ui::ui_instruction_t::TABITEM && BeginTabItem(inst->tabitem_title.c_str()))
                {
                    auto it = tab_content.find(inst->tabitem_title);
                    if (it != tab_content.end())
                    {
                        for (auto* ci : it->second)
                            render_instruction(ci, window_name);
                    }
                    EndTabItem();
                }
            }
            EndTabBar();
        }
        for (auto* inst : instructions)
        {
            if (inst->parent_tabbar.empty() && inst->type != module::core::graphics::ui::ui_instruction_t::TABBAR)
                render_instruction(inst, window_name);
        }
        End();
    }
}

namespace
{
    const ImU32 kTextStrong = IM_COL32(232, 232, 234, 255);
    const ImU32 kTextMuted = IM_COL32(138, 139, 143, 255);
    const ImU32 kSurfacePressed = IM_COL32(255, 255, 255, 220);

    // Every icon is a cell of the Hugeicons atlas uploaded by the renderer.
    void draw_icon(int icon, ImVec2 pos, float size, ImU32 tint)
    {
        void* atlas = module::core::graphics::ui::c_ui::icon_atlas;
        if (!atlas)
            return;

        const float atlas_width = (float)module::core::graphics::ui::icon_atlas_width;
        const float u0 = (icon * module::core::graphics::ui::icon_atlas_cell) / atlas_width;
        const float u1 = ((icon + 1) * module::core::graphics::ui::icon_atlas_cell) / atlas_width;

        ImGui::GetWindowDrawList()->AddImage((ImTextureID)(intptr_t)atlas, pos,
            ImVec2(pos.x + size, pos.y + size), ImVec2(u0, 0.0f), ImVec2(u1, 1.0f), tint);
    }

    // Navigation row: a hairline marker on the left when selected, nothing else.
    bool nav_row(const char* id, const char* label, int icon, bool active, float row_height)
    {
        const float scale = module::core::graphics::c_graphics::dpi_scale;
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImVec2 pos = window->DC.CursorPos;
        float width = window->WorkRect.Max.x - window->DC.CursorPos.x;

        ImGui::InvisibleButton(id, ImVec2(width, row_height));
        const bool hovered = ImGui::IsItemHovered();
        const bool clicked = ImGui::IsItemClicked(ImGuiMouseButton_Left);

        if (active)
            window->DrawList->AddRectFilled(pos, ImVec2(pos.x + width, pos.y + row_height), IM_COL32(255, 255, 255, 14), 4.0f);
        else if (hovered)
            window->DrawList->AddRectFilled(pos, ImVec2(pos.x + width, pos.y + row_height), IM_COL32(255, 255, 255, 8), 4.0f);

        if (active)
            window->DrawList->AddRectFilled(ImVec2(pos.x, pos.y + 6.0f * scale), ImVec2(pos.x + 2.0f * scale, pos.y + row_height - 6.0f * scale),
                kSurfacePressed, 1.0f);

        const float icon_size = 17.0f * scale;
        const float pad = 14.0f * scale;
        draw_icon(icon, ImVec2(pos.x + pad, pos.y + (row_height - icon_size) * 0.5f), icon_size,
            active ? kTextStrong : IM_COL32(132, 133, 138, 255));

        ImVec2 text_size = ImGui::CalcTextSize(label);
        window->DrawList->AddText(ImVec2(pos.x + pad + icon_size + 11.0f * scale, pos.y + (row_height - text_size.y) * 0.5f),
            active ? kTextStrong : kTextMuted, label);
        return clicked;
    }

    // Flat button: white fill for the primary action, hairline outline otherwise.
    bool icon_button(const char* label, int icon, float width, float height, bool primary)
    {
        const float scale = module::core::graphics::c_graphics::dpi_scale;

        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.64f, 0.5f));
        if (primary)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.933f, 0.933f, 0.937f, 1.00f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.84f, 0.84f, 0.85f, 1.00f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.10f, 0.10f, 0.10f, 1.00f));
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.00f, 1.00f, 1.00f, 0.04f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.00f, 1.00f, 1.00f, 0.09f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.00f, 1.00f, 1.00f, 0.14f));
        }

        const bool clicked = ImGui::Button(label, ImVec2(width, height));

        if (!primary)
        {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 255, 26), 4.0f);
        }

        ImGui::PopStyleColor(primary ? 4 : 3);
        ImGui::PopStyleVar();

        ImVec2 minimum = ImGui::GetItemRectMin();
        ImVec2 maximum = ImGui::GetItemRectMax();
        const float icon_size = 15.0f * scale;
        draw_icon(icon, ImVec2(minimum.x + 12.0f * scale, minimum.y + (maximum.y - minimum.y - icon_size) * 0.5f),
            icon_size, primary ? IM_COL32(18, 18, 18, 235) : IM_COL32(226, 226, 228, 235));
        return clicked;
    }
}

void module::core::graphics::ui::c_ui::draw()
{
    using namespace ImGui;
    static int active_panel = 0;
    static bool auto_scroll = true;
    static char filter_buf[64] = "";

    const float scale = c_graphics::dpi_scale;
    const float header_height = 52.0f * scale;
    const float footer_height = 26.0f * scale;
    const float nav_width = 168.0f * scale;
    const float row_height = 34.0f * scale;
    const float button_height = 32.0f * scale;

    SetNextWindowSizeConstraints(ImVec2(440.0f * scale, 280.0f * scale), ImVec2(FLT_MAX, FLT_MAX));

    // First appearance: centre the panel and never let it hang over the edge of
    // a small game window.
    static bool placed_once = false;
    if (!placed_once)
    {
        placed_once = true;
        ImGuiViewport* viewport = GetMainViewport();
        ImVec2 desired = ImVec2(780.0f * scale, 470.0f * scale);
        if (desired.x > viewport->WorkSize.x - 40.0f * scale)
            desired.x = ImMax(440.0f * scale, viewport->WorkSize.x - 40.0f * scale);
        if (desired.y > viewport->WorkSize.y - 40.0f * scale)
            desired.y = ImMax(280.0f * scale, viewport->WorkSize.y - 40.0f * scale);
        SetNextWindowSize(desired, ImGuiCond_Always);
        SetNextWindowPos(ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x - desired.x) * 0.5f,
            viewport->WorkPos.y + (viewport->WorkSize.y - desired.y) * 0.5f), ImGuiCond_Always);
    }
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    if (Begin(("##paradox_root"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse))
    {
        ImDrawList* draw_list = GetWindowDrawList();
        ImVec2 origin = GetWindowPos();
        ImVec2 size = GetWindowSize();
        const float pad = 18.0f * scale;

        // ---- header ---------------------------------------------------------
        {
            const float badge = 20.0f * scale;
            draw_icon(ICON_TERMINAL, ImVec2(origin.x + pad, origin.y + (header_height - badge) * 0.5f), badge, kTextStrong);

            draw_list->AddText(GetFont(), GetFontSize() + 1.0f,
                ImVec2(origin.x + pad + badge + 12.0f * scale, origin.y + header_height * 0.5f - GetFontSize() * 0.62f),
                kTextStrong, "Paradox");

            const char* version = "v1.0";
            float version_x = origin.x + size.x - pad - CalcTextSize(version).x;
            draw_list->AddText(ImVec2(version_x, origin.y + header_height * 0.5f - GetFontSize() * 0.5f), kTextMuted, version);

            SetCursorScreenPos(origin);
            InvisibleButton(("##header_drag"), ImVec2(size.x, header_height));
            if (IsItemActive() && IsMouseDragging(ImGuiMouseButton_Left))
            {
                ImVec2 delta = GetIO().MouseDelta;
                ImVec2 current = GetWindowPos();
                SetWindowPos(ImVec2(current.x + delta.x, current.y + delta.y));
            }

            draw_list->AddLine(ImVec2(origin.x, origin.y + header_height),
                ImVec2(origin.x + size.x, origin.y + header_height), IM_COL32(255, 255, 255, 14), 1.0f);
        }

        // ---- sidebar --------------------------------------------------------
        const float body_height = size.y - header_height - footer_height;
        SetCursorPos(ImVec2(0.0f, header_height));
        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f * scale, 12.0f * scale));
        BeginChild(("##nav"), ImVec2(nav_width, body_height), false, ImGuiWindowFlags_NoScrollbar);
        {
            struct nav_entry { const char* label; int icon; };
            static const nav_entry entries[] = {
                { ("Execution"), ICON_PLAY },
                { ("Scripts"), ICON_CODE },
                { ("Output"), ICON_TERMINAL },
                { ("Settings"), ICON_SETTINGS },
            };
            for (int i = 0; i < IM_ARRAYSIZE(entries); i++)
            {
                char nav_id[16];
                snprintf(nav_id, sizeof(nav_id), "##nav%d", i);
                if (nav_row(nav_id, entries[i].label, entries[i].icon, active_panel == i, row_height))
                    active_panel = i;
            }
        }
        EndChild();
        PopStyleVar();

        // ---- content --------------------------------------------------------
        // hairline divider between the navigation rail and the panel
        draw_list->AddLine(ImVec2(origin.x + nav_width, origin.y + header_height + 8.0f * scale),
            ImVec2(origin.x + nav_width, origin.y + size.y - footer_height - 8.0f * scale),
            IM_COL32(255, 255, 255, 12), 1.0f);
        SameLine();
        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16.0f * scale, 14.0f * scale));
        BeginChild(("##content"), ImVec2(0.0f, body_height), false);
        {
            if (active_panel == 0)
            {
                if (BeginTabBar(("##scripttabs"), ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable))
                {
                    if (TabItemButton(("+"), ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
                        add_tab();
                    int tab_to_close = -1;
                    for (int i = 0; i < (int)editor_tabs.size(); i++)
                    {
                        auto& tab = editor_tabs[i];
                        bool is_open = true;
                        bool* p_open = editor_tabs.size() > 1 ? &is_open : nullptr;
                        if (BeginTabItem(tab.name.c_str(), p_open))
                        {
                            active_tab = i;
                            EndTabItem();
                        }
                        if (p_open && !is_open)
                            tab_to_close = i;
                    }
                    EndTabBar();
                    if (tab_to_close != -1)
                        close_tab(tab_to_close);
                }

                const float editor_height = GetContentRegionAvail().y - button_height - 12.0f * scale;
                if (active_tab >= 0 && active_tab < (int)editor_tabs.size())
                    editor_tabs[active_tab].editor.Render(("##editor"), ImVec2(0.0f, editor_height), true);

                SetCursorPosY(GetCursorPosY() + 8.0f * scale);
                if (icon_button(("Execute"), ICON_PLAY, 128.0f * scale, button_height, true) &&
                    active_tab >= 0 && active_tab < (int)editor_tabs.size())
                    execute_script(editor_tabs[active_tab].editor.GetText());
                SameLine();
                if (icon_button(("Clear"), ICON_DELETE, 108.0f * scale, button_height, false) &&
                    active_tab >= 0 && active_tab < (int)editor_tabs.size())
                    editor_tabs[active_tab].editor.SetText((""));
                SameLine();
                if (icon_button(("Clipboard"), ICON_CLIPBOARD, 130.0f * scale, button_height, false))
                {
                    const char* clip = GetClipboardText();
                    if (clip)
                        execute_script(clip);
                }
            }
            else if (active_panel == 1)
            {
                TextDisabled(("Curated scripts"));
                SetNextItemWidth(-1.0f);
                Combo(("##script"), &current_script, script_labels, IM_ARRAYSIZE(script_labels));
                SetCursorPosY(GetCursorPosY() + 10.0f * scale);
                if (icon_button(("Execute"), ICON_PLAY, 128.0f * scale, button_height, true))
                    execute_script(script_sources[current_script]);
                SameLine();
                if (icon_button(("Copy"), ICON_COPY, 100.0f * scale, button_height, false))
                    SetClipboardText(script_sources[current_script]);
                SameLine();
                if (icon_button(("New tab"), ICON_PLUS, 116.0f * scale, button_height, false))
                    add_tab(script_labels[current_script], script_sources[current_script]);
            }
            else if (active_panel == 2)
            {
                if (icon_button(("Clear"), ICON_DELETE, 100.0f * scale, button_height, false))
                    log_mgr.clear_log_info();
                SameLine();
                if (icon_button(("Copy"), ICON_COPY, 96.0f * scale, button_height, false))
                {
                    auto logs = log_mgr.get_log_info();
                    std::string out;
                    for (const auto& l : logs)
                    {
                        out += l.content;
                        out += '\n';
                    }
                    SetClipboardText(out.c_str());
                }
                SameLine();
                Checkbox(("Auto scroll"), &auto_scroll);
                SameLine();
                SetNextItemWidth(180.0f * scale);
                InputTextWithHint(("##filter"), ("Filter..."), filter_buf, sizeof(filter_buf));
                Separator();
                ImVec2 avail = GetContentRegionAvail();
                if (BeginChild(("##output_scroll"), avail, false, ImGuiWindowFlags_HorizontalScrollbar))
                {
                    auto logs = log_mgr.get_log_info();
                    const char* filter = filter_buf;
                    const bool has_filter = filter[0] != '\0';
                    for (const auto& l : logs)
                    {
                        if (!l.visible) continue;
                        if (has_filter && l.content.find(filter) == std::string::npos)
                            continue;
                        PushStyleColor(ImGuiCol_Text, l.color);
                        TextUnformatted(l.content.c_str());
                        PopStyleColor();
                    }
                    if (auto_scroll && GetScrollY() >= GetScrollMaxY() - 10.0f)
                        SetScrollHereY(1.0f);
                }
                EndChild();
            }
            else
            {
                TextDisabled(("About"));
                SetCursorPosY(GetCursorPosY() + 6.0f * scale);
                Text(("Toggle the menu with INSERT, END or F2."));
                SetCursorPosY(GetCursorPosY() + 16.0f * scale);
                if (icon_button(("Unload"), ICON_STOP, 120.0f * scale, button_height, false))
                    exit(0);
            }
        }
        EndChild();
        PopStyleVar();

        // ---- footer ---------------------------------------------------------
        {
            float y = origin.y + size.y - footer_height;
            draw_list->AddLine(ImVec2(origin.x, y), ImVec2(origin.x + size.x, y), IM_COL32(255, 255, 255, 14), 1.0f);

            char status[96];
            snprintf(status, sizeof(status), "%.0f FPS", GetIO().Framerate);
            float status_width = CalcTextSize(status).x;
            float text_y = y + (footer_height - GetFontSize()) * 0.5f;
            draw_list->AddText(ImVec2(origin.x + pad, text_y), kTextMuted, "Ready");
            draw_list->AddText(ImVec2(origin.x + size.x - pad - status_width, text_y), kTextMuted, status);
        }
    }
    End();
    PopStyleVar();
}
