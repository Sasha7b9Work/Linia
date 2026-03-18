// 2023/08/09 19:11:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Utils/Configurator.h"


namespace SET
{
    namespace GUI
    {
        ValuePoint       position("gui_position", { 100, 100 });
        ValuePoint       pos_console("gui_pos_console", { 10, 10 });
        ValuePoint       size_console("gui_size_console", { 400, 400 });
        ValuePoint       pos_list("gui_pos_list", { 10, 10 });
        ValuePoint       size_list("gui_size_list", { 400, 400 });
        ValueBool        maximized_list("gui_maximized_list", false);
        ValueBool        maximized_console("gui_maximized_console", false);
        ValueInt         current_page_notebook("gui_current_page_notebook", 0);
        ValueInt         serial_port_num("serial_port_num", 0);
        ValueBool        debug_mode("debug_mode", false);

        ValueUInt        color_background("color_background", 0xFFFFFF);
        ValueUInt        color_grid("color_grid", 0);
        ValueUInt        color_font("color_font", 0);
        ValueUInt        color_curve("color_curve", 0);
        ValueUInt        color_link("color_link", 0);
        ValueUInt        color_secant("color_secant", 0);

        ValueInt         size_point("size_point", 2);
        ValueBool        cursors_x("cursors_x", false);
        ValueBool        cursors_y("cursors_y", false);
        ValueBool        track_x("track_x", false);
        ValueBool        track_y("track_y", false);
        ValueBool        track_mouse("track_mouse", false);
        ValuePoint       calculation_pos{ "calculation_pos", {10, 10} };
        ValuePoint       cursors_pos{ "cursors_pos", {10, 10} };
    }

    static std::vector<ValuePoint *> &VectorPoint()
    {
        static std::vector<ValuePoint *> vec;
        return vec;
    }
    static std::vector<ValueBool *> values_bool;
    static std::vector<ValueInt *> values_int;
    static std::vector<ValueUInt *> values_uint;
}


void SET::AppendValue(ValuePoint *value)
{
    VectorPoint().push_back(value);
}


void SET::AppendValue(ValueBool *value)
{
    values_bool.push_back(value);
}


void SET::AppendValue(ValueInt *value)
{
    values_int.push_back(value);
}


void SET::AppendValue(ValueUInt *value)
{
    values_uint.push_back(value);
}


void SET::GUI::Load()
{
    for (auto elem : VectorPoint())
    {
        elem->Load();
    }

    for (auto elem : values_bool)
    {
        elem->Load();
    }

    for (auto elem : values_int)
    {
        elem->Load();
    }

    for (auto elem : values_uint)
    {
        elem->Load();
    }
}


void SET::GUI::Save()
{
    for (auto elem : VectorPoint())
    {
        elem->Save();
    }

    for (auto elem : values_bool)
    {
        elem->Save();
    }

    for (auto elem : values_int)
    {
        elem->Save();
    }

    for (auto elem : values_uint)
    {
        elem->Save();
    }
}
