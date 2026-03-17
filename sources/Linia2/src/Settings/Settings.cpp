// 2023/08/09 19:11:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Utils/Configurator.h"


namespace SET
{
    namespace GUI
    {
        Value<wxPoint>   position("gui_position", { 100, 100 });
        Value<wxPoint>   pos_console("gui_pos_console", { 10, 10 });
        Value<wxPoint>   size_console("gui_size_console", { 400, 400 });
        Value<wxPoint>   pos_list("gui_pos_list", { 10, 10 });
        Value<wxPoint>   size_list("gui_size_list", { 400, 400 });
        Value<bool>      maximized_list("gui_maximized_list", false);
        Value<bool>      maximized_console("gui_maximized_console", false);
        Value<int>       current_page_notebook("gui_current_page_notebook", 0);
        Value<int>       serial_port_num("serial_port_num", 0);
        Value<bool>      debug_mode("debug_mode", false);

        Value<uint>      color_background("color_background", 0xFFFFFF);
        Value<uint>      color_grid("color_grid", 0);
        Value<uint>      color_font("color_font", 0);
        Value<uint>      color_curve("color_curve", 0);
        Value<uint>      color_link("color_link", 0);
        Value<uint>      color_secant("color_secant", 0);

        Value<int>       size_point("size_point", 2);
        Value<bool>      track_x("track_x", false);
        Value<bool>      track_y("track_y", false);
        Value<bool>      track_mouse("track_mouse", false);
        Value<wxPoint>   calculation_pos{ "calculation_pos", {10, 10} };
        Value<wxPoint>   cursors_pos{ "cursors_pos", {10, 10} };
    }
}


void SET::GUI::Load()
{
    position.Load();
    pos_console.Load();
    size_console.Load();
    pos_list.Load();
    size_list.Load();
    maximized_list.Load();
    debug_mode.Load();
    maximized_console.Load();
    current_page_notebook.Load();
    serial_port_num.Load();

    color_background.Load();
    color_grid.Load();
    color_font.Load();
    color_curve.Load();
    color_link.Load();
    color_secant.Load();

    size_point.Load();
    track_x.Load();
    track_y.Load();
    track_mouse.Load();
    calculation_pos.Load();
    cursors_pos.Load();
}


void SET::GUI::Save()
{
    position.Save();
    pos_console.Save();
    size_console.Save();
    pos_list.Save();
    size_list.Save();
    maximized_list.Save();
    debug_mode.Save();
    maximized_console.Save();
    current_page_notebook.Save();
    serial_port_num.Save();

    color_background.Save();
    color_grid.Save();
    color_font.Save();
    color_curve.Save();
    color_link.Save();
    color_secant.Save();

    size_point.Save();
    track_x.Save();
    track_y.Save();
    track_mouse.Save();
    calculation_pos.Save();
    cursors_pos.Save();
}
