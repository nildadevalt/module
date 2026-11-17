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
    const ImU32 kAccent = IM_COL32(124, 105, 245, 255);
    const ImU32 kAccentDim = IM_COL32(124, 105, 245, 190);
    const ImU32 kTextStrong = IM_COL32(240, 241, 246, 255);
    const ImU32 kTextMuted = IM_COL32(150, 154, 168, 255);

    // Flat, rounded navigation row: the active one carries a muted accent pill,
    // nothing else. No borders, no gradients.
    bool nav_row(const char* id, const char* label, bool active, float row_height)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImVec2 pos = window->DC.CursorPos;
        float width = window->WorkRect.Max.x - window->DC.CursorPos.x;

        ImGui::InvisibleButton(id, ImVec2(width, row_height));
        const bool hovered = ImGui::IsItemHovered();
        const bool clicked = ImGui::IsItemClicked(ImGuiMouseButton_Left);

        ImU32 background = active ? kAccentDim : (hovered ? IM_COL32(255, 255, 255, 16) : 0);
        if (background)
            window->DrawList->AddRectFilled(pos, ImVec2(pos.x + width, pos.y + row_height), background, 8.0f);

        ImVec2 text_size = ImGui::CalcTextSize(label);
        window->DrawList->AddText(ImVec2(pos.x + 14.0f, pos.y + (row_height - text_size.y) * 0.5f),
            active ? IM_COL32(255, 255, 255, 255) : kTextMuted, label);
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
    const float header_height = 46.0f * scale;
    const float footer_height = 26.0f * scale;
    const float nav_width = 156.0f * scale;
    const float row_height = 32.0f * scale;
    const float button_height = 30.0f * scale;

    SetNextWindowSize(ImVec2(760.0f * scale, 460.0f * scale), ImGuiCond_FirstUseEver);
    SetNextWindowSizeConstraints(ImVec2(420.0f * scale, 260.0f * scale), ImVec2(FLT_MAX, FLT_MAX));
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    if (Begin(("##paradox_root"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse))
    {
        ImDrawList* draw_list = GetWindowDrawList();
        ImVec2 origin = GetWindowPos();
        ImVec2 size = GetWindowSize();

        // ---- header ---------------------------------------------------------
        {
            float pad = 18.0f * scale;
            draw_list->AddRectFilled(ImVec2(origin.x + pad, origin.y + 15.0f * scale),
                ImVec2(origin.x + pad + 10.0f * scale, origin.y + 25.0f * scale), kAccent, 3.0f);
            draw_list->AddText(GetFont(), GetFontSize(), ImVec2(origin.x + pad + 20.0f * scale, origin.y + 11.0f * scale),
                kTextStrong, "Paradox");

            const char* subtitle = "executor";
            float subtitle_width = CalcTextSize(subtitle).x;
            draw_list->AddText(ImVec2(origin.x + size.x - pad - subtitle_width, origin.y + 12.0f * scale),
                kTextMuted, subtitle);

            SetCursorScreenPos(ImVec2(origin.x, origin.y));
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
            struct nav_entry { const char* label; };
            static const nav_entry entries[] = {
                { ("\xEE\x9D\xA8  Execution") },
                { ("\xEE\x9F\x83  Scripts") },
                { ("\xEE\xA2\xA5  Output") },
                { ("\xEE\x9C\x93  Settings") },
            };
            for (int i = 0; i < IM_ARRAYSIZE(entries); i++)
            {
                char nav_id[16];
                snprintf(nav_id, sizeof(nav_id), "##nav%d", i);
                if (nav_row(nav_id, entries[i].label, active_panel == i, row_height))
                    active_panel = i;
            }
        }
        EndChild();
        PopStyleVar();

        // ---- content --------------------------------------------------------
        SameLine();
        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16.0f * scale, 14.0f * scale));
        BeginChild(("##content"), ImVec2(0.0f, body_height), false);
        {
            const ImVec4 primary = ImVec4(124.0f / 255.0f, 105.0f / 255.0f, 245.0f / 255.0f, 1.0f);

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

                const float editor_height = GetContentRegionAvail().y - button_height - 10.0f * scale;
                if (active_tab >= 0 && active_tab < (int)editor_tabs.size())
                    editor_tabs[active_tab].editor.Render(("##editor"), ImVec2(0.0f, editor_height), true);

                SetCursorPosY(GetCursorPosY() + 6.0f * scale);
                PushStyleColor(ImGuiCol_Button, primary);
                PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(primary.x * 1.12f, primary.y * 1.12f, primary.z * 1.12f, 1.0f));
                PushStyleColor(ImGuiCol_ButtonActive, ImVec4(primary.x * 0.9f, primary.y * 0.9f, primary.z * 0.9f, 1.0f));
                if (Button(("\xEE\x9D\xA8  Execute"), ImVec2(120.0f * scale, button_height)) && active_tab >= 0 && active_tab < (int)editor_tabs.size())
                    execute_script(editor_tabs[active_tab].editor.GetText());
                PopStyleColor(3);
                SameLine();
                if (Button(("\xEE\x9D\x8D  Clear"), ImVec2(100.0f * scale, button_height)) && active_tab >= 0 && active_tab < (int)editor_tabs.size())
                    editor_tabs[active_tab].editor.SetText((""));
                SameLine();
                if (Button(("\xEE\x9D\xBF  Clipboard"), ImVec2(120.0f * scale, button_height)))
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
                SetCursorPosY(GetCursorPosY() + 8.0f * scale);
                PushStyleColor(ImGuiCol_Button, primary);
                PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(primary.x * 1.12f, primary.y * 1.12f, primary.z * 1.12f, 1.0f));
                if (Button(("\xEE\x9D\xA8  Execute"), ImVec2(120.0f * scale, button_height)))
                    execute_script(script_sources[current_script]);
                PopStyleColor(2);
                SameLine();
                if (Button(("\xEE\xA3\x88  Copy"), ImVec2(100.0f * scale, button_height)))
                    SetClipboardText(script_sources[current_script]);
                SameLine();
                if (Button(("\xEE\x9C\x90  New tab"), ImVec2(110.0f * scale, button_height)))
                    add_tab(script_labels[current_script], script_sources[current_script]);
            }
            else if (active_panel == 2)
            {
                if (Button(("\xEE\x9D\x8D  Clear"), ImVec2(90.0f * scale, button_height)))
                    log_mgr.clear_log_info();
                SameLine();
                if (Button(("\xEE\xA3\x88  Copy"), ImVec2(90.0f * scale, button_height)))
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
                TextDisabled(("Paradox"));
                SetCursorPosY(GetCursorPosY() + 4.0f * scale);
                Text(("Toggle the menu with INSERT, END or F2."));
                SetCursorPosY(GetCursorPosY() + 12.0f * scale);
                if (Button(("Unload"), ImVec2(110.0f * scale, button_height)))
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
            draw_list->AddText(ImVec2(origin.x + 18.0f * scale, y + 5.0f * scale), kTextMuted, "Ready");
            draw_list->AddText(ImVec2(origin.x + size.x - 18.0f * scale - status_width, y + 5.0f * scale), kTextMuted, status);
        }
    }
    End();
    PopStyleVar();
}
