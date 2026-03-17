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
        extern Value<bool>      maximized_list;
        extern Value<bool>      maximized_console;
        extern Value<int>       current_page_notebook;
        extern Value<int>       serial_port_num;            // Нумерация начинается с нуля. 0 == COM1
        extern Value<bool>      debug_mode;

        extern Value<uint>      color_background;
        extern Value<uint>      color_grid;
        extern Value<uint>      color_font;
        extern Value<uint>      color_curve;                // Цвет графиков
        extern Value<uint>      color_link;
        extern Value<uint>      color_secant;               // Цвет секущей

        extern Value<int>       size_point;                 // Размер точки на графике
        extern Value<bool>      track_x;                    // Отслеживать значения измерений по Х
        extern Value<bool>      track_y;                    // и Y
        extern Value<bool>      track_mouse;                // и положение мыши
        extern Value<wxPoint>   calculation_pos;            // Позиция окна РАСЧЁТ
        extern Value<wxPoint>   cursors_pos;                // Позиция окна КУРСОРЫ
    }
};
