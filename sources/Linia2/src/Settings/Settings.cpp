// 2023/08/09 19:11:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Utils/Configurator.h"


namespace SET
{
    namespace GUI
    {
        ValuePoint       pos_console("gui_pos_console", { 10, 10 });
        ValuePoint       size_console("gui_size_console", { 400, 400 });
        ValueBool        maximized_console("gui_maximized_console", false);
        ValueInt         serial_port_num("serial_port_num", 0);
        ValueBool        debug_mode("debug_mode", false);
        ValueBool        emulate_mode("emulate_mode", false);

        ValueUInt        color_background("color_background", 0xFFFFFF);
        ValueUInt        color_grid("color_grid", 0);
        ValueUInt        color_font("color_font", 0);
        ValueUInt        color_curve("color_curve", 0);
        ValueUInt        color_link("color_link", 0);
        ValueUInt        color_secant("color_secant", 0);

        ValueInt         size_point("size_point", 2);
        Value<wxString>  current_panel{ "current_panel", "PanelChannelC" };

        void Load();
        void Save();
    }


    // \warn Такое хитрое обращение к векторам сделано потому, что порядок инициализации глобальных статических объектов неопределён,
    // из-за чего к моменту, когда в эти вектора кладутся значения, оин могут быть не готовы к работе. 
    static std::vector<ValuePoint *> &VectorPoint()
    {
        static std::vector<ValuePoint *> vec;
        return vec;
    }

    static std::vector<ValueBool *> &VectorBool()
    {
        static std::vector<ValueBool *> vec;
        return vec;
    }

    static std::vector<ValueInt *> &VectorInt()
    {
        static std::vector<ValueInt *> vec;
        return vec;
    }

    static std::vector<ValueUInt *> &VectorUInt()
    {
        static std::vector<ValueUInt *> vec;
        return vec;
    }
}


void SET::AppendValue(ValuePoint *value)
{
    VectorPoint().push_back(value);
}


void SET::AppendValue(ValueBool *value)
{
    VectorBool().push_back(value);
}


void SET::AppendValue(ValueInt *value)
{
    VectorInt().push_back(value);
}


void SET::AppendValue(ValueUInt *value)
{
    VectorUInt().push_back(value);
}


void SET::Load()
{
    for (auto elem : VectorPoint())
    {
        elem->Load();
    }

    for (auto elem : VectorBool())
    {
        elem->Load();
    }

    for (auto elem : VectorInt())
    {
        elem->Load();
    }

    for (auto elem : VectorUInt())
    {
        elem->Load();
    }

    GUI::Load();
}


void SET::Save()
{
    for (auto elem : VectorPoint())
    {
        elem->Save();
    }

    for (auto elem : VectorBool())
    {
        elem->Save();
    }

    for (auto elem : VectorInt())
    {
        elem->Save();
    }

    for (auto elem : VectorUInt())
    {
        elem->Save();
    }

    GUI::Save();
}


void SET::GUI::Load()
{
    current_panel.Load();
}


void SET::GUI::Save()
{
    current_panel.Save();
}
