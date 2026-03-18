// 2023/08/09 19:11:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Value.h"


// Настройки прибора
namespace SET
{
    void AppendValue(ValuePoint *);
    void AppendValue(ValueBool *);
    void AppendValue(ValueInt *);
    void AppendValue(ValueUInt *);

    // Настройки ГУИ
    namespace GUI
    {
        void Load();
        void Save();

        extern ValuePoint       pos_console;
        extern ValuePoint       size_console;
        extern ValuePoint       pos_list;
        extern ValuePoint       size_list;
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
        extern ValuePoint       calculation_pos;            // Позиция окна РАСЧЁТ
    }
};
