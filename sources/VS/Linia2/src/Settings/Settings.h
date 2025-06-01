// 2023/08/09 19:11:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Value.h"


struct Password
{
    uint64   value;
    pchar    key;               // Для сохранения в файле
    wxString ToString() const;
};


struct Key128
{
    BitSet128 bitset;
};


struct BaudRateOSDP
{
    static uint FromIndexCombobox(int index);
};


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

    // Дополнительно - Автономный режим
    namespace OFFLINE
    {
        extern ValueCheckButton  enabled;               // Вкл/выкл
        extern ValueCheckBox     security_mode;         // Режим охраны
        extern ValueTextCtrl     time_lock;             // Время замка
        extern ValueTextCtrl     time_alarm;            // Время тревоги
        extern Value<wxString>   file_cards;            // Список карт автономного режима
    }

    namespace WIEGAND
    {
        static const int        value_min = 26;
        static const int        value_max = 64;
        extern ValueTextCtrl    value;                  // Wiegand
        extern ValueCheckBox    full_guid;              // Передавать ли полный GUID
        extern ValueCheckBox    control_bits;           // Считать ли контрольные биты
        extern ValueCheckBox    inverse_control_bits;   // Обратный порядок расчёта контрольных бит - чёт/нечет поменян местами
        extern ValueCheckBox    reverse_order_bits;     // Обратный порядок бит
        extern ValueCheckBox    nuid_discard_LSB;       // Отрасывать в NUID младший значащий бит

        // Возвращает полное значение для передачи для записи мастер-карты
        // b0 - value, b1 - control_bits, b2 - inverse
        uint GetFullValue();
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


struct Settings
{
    // Карта
    Password password_card;     // Текущий пароль карты

    // Пароль
    Password password_master;   // Пароль к мастер-карте            старый
    Password password_current;  // Действующий пароль системы       новый

//    Password key_crypto_OSDP;   // Пароль на шифрование OSDP

    Key128 keysSL0[5];  // 9001...90004
};


extern Settings gset;
extern const Password password_factory;  // Пароль к мастер-карте - Заводской пароль
