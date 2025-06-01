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

    namespace MASTER
    {
        extern ValueColor color_red;
        extern ValueColor color_green;

        // Конфигурация
        extern ValueComboBox melody[3];                 // Звуки - Тип
        extern ValueSpinCtrl volume[3];                 // Звуки - Громкость

        // Запись на мастер-карту
        extern ValueCheckBox write_config;              // Конфигураций
    }

    // Дополнительно - Режим OSDP
    namespace OSDP
    {
        extern ValueCheckButton  enabled;               // Вкл/выкл
        extern ValueTextCtrl     address;               // Адрес
        extern ValueComboBox     baudrate;              // Скорость
        extern ValueCheckBox     crypto_enabled;        // Включение шифрование
    }

    // Дополнительно - Датчик отрыва
    namespace ANTIBREAK
    {
        extern ValueCheckButton  enabled;               // Вкл/Откл
        extern ValueTextCtrl     number;                // Номер
        extern ValueComboBox     sens;                  // Чувствительность

        uint8 GetRAW();                                 // Возвращает значение для записи в мастер карту
                                                        // Фактически чем больше это число, тем меньше чувствительность. 0 - отключено
    }

    // Расширенные настройки
    namespace EXT
    {
        extern ValueCheckButton  enabled;               // Вкл/Выкл
        extern ValueCheckBox     parity;                // Паритет
        extern ValueCheckBox     inverse_code;          // Инверсия выдачи кода
        extern ValueCheckBox     control_bit;           // Контрольный бит
        extern ValueTextCtrl     period_autorepeat;     // Период автоповтора
        extern ValueComboBox     mode_read_card;        // Режим чтения карты
    }

    namespace MISC
    {
        extern ValueCheckBox    disable_less_SL3;       // Отключить карты ниже SL3
    }

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
