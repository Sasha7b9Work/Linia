// 2023/08/09 19:11:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Value.h"


// Настройки прибора
namespace SET
{
    void Init();
    void Load(const wxString &file_path);
    void Save(const wxString &file_path);

    ////////////////////////////////////////////////// Карты доступа

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
        extern ValueCheckButton serial_port_auto_find;
        extern Value<int>       serial_port_num;             // Нумерация начинается с нуля. 0 == COM1
        extern Value<bool>      debug_mode;
    }

};
