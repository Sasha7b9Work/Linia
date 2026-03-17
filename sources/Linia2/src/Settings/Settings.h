// 2023/08/09 19:11:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Value.h"


// Настройки прибора
namespace SET
{
    // Настройки ГУИ
    namespace GUI
    {
        void Load();
        void Save();
        extern Value<wxPoint>   position;
        extern Value<wxPoint>   pos_console;
        extern Value<wxPoint>   size_console;
        extern Value<wxPoint>   pos_list;
        extern Value<wxPoint>   size_list;
        extern ValueBool        maximized_list;
        extern ValueBool        maximized_console;
        extern ValueInt         current_page_notebook;
        extern ValueInt         serial_port_num;            // Нумерация начинается с нуля. 0 == COM1
        extern ValueBool        debug_mode;

        extern ValueUInt        color_background;
        extern ValueUInt        color_grid;
        extern ValueUInt        color_font;
        extern ValueUInt        color_curve;                // Цвет графиков
        extern ValueUInt        color_link;
        extern ValueUInt        color_secant;               // Цвет секущей

        extern ValueInt         size_point;                 // Размер точки на графике
        extern ValueBool        cursors_x;
        extern ValueBool        cursors_y;
        extern ValueBool        track_x;                    // Отслеживать значения измерений по Х
        extern ValueBool        track_y;                    // и Y
        extern ValueBool        track_mouse;                // и положение мыши
        extern Value<wxPoint>   calculation_pos;            // Позиция окна РАСЧЁТ
        extern Value<wxPoint>   cursors_pos;                // Позиция окна КУРСОРЫ
    }
};
