// 2026/08/28 15:06:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Settings/Settings.h"
#include <map>

namespace SET
{
    static std::map<std::string, IValue *> &GetRegistry()
    {
        static std::map<std::string, IValue *> registry;
        return registry;
    }

    void RegisterValue(const std::string &name, IValue *value)
    {
        GetRegistry()[name] = value;
    }

    IValue *GetValue(const std::string &name)
    {
        auto it = GetRegistry().find(name);
        if (it != GetRegistry().end())
        {
            return it->second;
        }
        return nullptr;
    }

    void Init()
    {
#define REGISTER(type, name, key, def) \
            auto* name##_ptr = new Value<type>(key, def); \
            RegisterValue(#name, new ValueWrapper<type>(name##_ptr));

        REGISTER(bool, debug_mode, "debug_mode", false);
        REGISTER(bool, debug_emulate_mode, "debug_emulate_mode", false);
        REGISTER(wxPoint, gui_pos_console, "gui_pos_console", wxPoint(10, 10));
        REGISTER(wxPoint, gui_size_console, "gui_size_console", wxPoint(400, 400));
        REGISTER(bool, gui_maximized_console, "gui_maximized_console", false);
        REGISTER(int, gui_serial_port_num, "gui_serial_port_num", 0);
        REGISTER(uint, gui_color_background, "gui_color_background", 0xFFFFFF);
        REGISTER(uint, gui_color_grid, "gui_color_grid", 0);
        REGISTER(uint, gui_color_font, "gui_color_font", 0);
        REGISTER(uint, gui_color_curve, "gui_color_curve", 0);
        REGISTER(uint, gui_color_link, "gui_color_link", 0);
        REGISTER(uint, gui_color_secant, "gui_color_secant", 0);
        REGISTER(int, gui_size_point, "gui_size_point", 2);
        REGISTER(int, current_panel, "current_panel", 0);
        REGISTER(uint, period_send, "period_send", 100);

#undef REGISTER
    }

    void Load()
    {
        for (auto &[name, value] : GetRegistry())
        {
            value->Load();
        }
    }

    void Save()
    {
        for (auto &[name, value] : GetRegistry())
        {
            value->Save();
        }
    }
}
