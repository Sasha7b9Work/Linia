// 2023/08/09 19:11:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Value.h"


// Настройки прибора
namespace SET
{
    void Init();
    void Load(const wxString &file_path);
    void Save(const wxString &file_path);

    extern ValueCheckButton mode_eco;

    ////////////////////////////////////////////////// Карты доступа

    namespace USER
    {
        // Генерация номера карты
        extern ValueTextCtrl    number_first;               // первый
        extern ValueTextCtrl    number_last;                // последний
        extern ValueTextCtrl    number_next;                // Номер следующей карты
        extern ValueCheckBox    enabled_range_generation;   // Вкл/вылк
        extern Value<wxString>  file_cards;                 // Список карт неавтономного режима

        // Запись на карту
        extern ValueCheckBox write_client_number;           // Нового номера карты
    }

    // Настройки ГУИ
    namespace GUI
    {
        void Load();
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
    }

};
