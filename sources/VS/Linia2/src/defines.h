// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "opt.h"


#define WIN32_LEAN_AND_MEAN
#pragma warning (push, 0)
#include <wx/config.h>
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/display.h>
#include <wx/tglbtn.h>
#include <wx/fileconf.h>
#include <wx/notebook.h>
#include <wx/listctrl.h>
#include <wx/statline.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <map>
#include <queue>
#include <mutex>
#include <stack>
#pragma warning(pop)


#include "Utils/Localizator.h"
#include "Utils/Log.h"


typedef const char *       pchar;
typedef const char *const  pString;
typedef unsigned char      uchar;
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef signed short       int16;
typedef unsigned int       uint;
typedef unsigned long long uint64;
typedef signed long long   int64;
typedef signed char        int8;


enum
{
    ID_TIMER = wxID_HIGHEST + 1,
    ID_TIMER_COLOR_DIALOG,

    ID_LINE,

    ID_BUTTON_LOAD_IMPORT,
    ID_BUTTON_LOAD_USER_CARDS,
    ID_BUTTON_LOAD_PASSWORDS,
    ID_BUTTON_LOAD_CONFIG,
    ID_BUTTON_SAVE_USER_CARDS,
    ID_BUTTON_SAVE_PASSWORDS,
    ID_BUTTON_SAVE_CONFIG,
    ID_BUTTON_SAVE_EXPORT,
    ID_BUTTON_OPEN_ARCHIVE_PASSWORDS,
    ID_BUTTON_OPEN_ARCHIVE_EVENTS,
    ID_BUTTON_WRITE_MASTER_CARD,                    // Запись на мастер карту
    ID_BUTTON_RED,
    ID_BUTTON_GREEN,
    ID_BUTTON_WRITE_USER_CARD,
    ID_BUTTON_CARD_INFO,
    ID_BUTTON_RESET_READER,
    ID_BUTTON_APPLY_PASSWORD,
    ID_BUTTON_READER_INFO,
    ID_BUTTON_UPGRADE_FIRMWARE,                     // "Информация о считывателе" - "Обновить прошивку"
    ID_BUTTON_FILE_UPGRADE,                         // "Информация о считывателе" - "Обновить прошивку" - "Выбрать файл"
    ID_BUTTON_FILE_SOUND,
    ID_BUTTON_LOAD_SOUND_START = ID_BUTTON_FILE_SOUND + SOUNDS_IN_MEMORY + 1,
    ID_BUTTON_UPGRADE_START,                        // "Информация о считывателе" - "Обновить прошивку" - "Начать обновление"
    ID_BUTTON_PASSWORD_MASTER_CARD,                 // Пароль - Пароль к мастер-карте
    ID_BUTTON_CURRENT_SYSTEM_PASSWORD,              // Действующий пароль системы
    ID_BUTTON_CLOSE_INFO_READER,
    ID_BUTTON_PASSWORD_EXTRA,                       // Пароль - Дополнительно
    ID_BUTTON_CLOSE_INFO_CARD,
    ID_BUTTON_LOAD_SOUND,
    ID_BUTTON_MELODY,
    ID_BUTTON_WIEGAND,
    ID_BUTTON_OFFLINE_MODE,                         // Кнопка настроек режима охраны
    ID_BUTTON_OSDP,                                 // Кнопка настроек режима OSDP
    ID_BUTTON_ANTIBREAK,                            // Кнопка анти-кражи
    ID_BUTTON_EXTRA,                                // Дополнительные настройи
    ID_BUTTON_EKO,
    ID_BUTTON_OPEN_OFFLINE_DATABASE,
    ID_BUTTON_FIND_PORTS,
    ID_BUTTON_KEYS_SL0,                             // Ключи для записи в SL0
    ID_BUTTON_KEYS_SL0_READ_FILE,                   // Прочитать ключи из файла
    ID_BUTTON_KEYS_SL0_MANUAL,                      // Ввести вручную
    ID_BUTTON_KEYS_SL0_SAVE_FILE,                   // Сохранить ключи в файл
    ID_BUTTON_KEYS_SL0_VIEW,                        // Посмотреть ключи
    ID_BUTTON_KEYS_SL0_WRITE_TO_CARD,               // Записать ключи в карту
    ID_BUTTON_KEYS_SL0_WRITE_TO_READER,             // Записать ключи в считыватель
    ID_BUTTON_CLOSE,
    ID_BUTTON_SAVE,

    ID_BUTTON_DECODE_MP3,

    ID_BUTTON_WRITE_KEYS_AES128,
    ID_BUTTON_WRITE_MASTERCARD_KEYS,

    ID_COMBOBOX_MELODY,
    ID_COMBOBOX_EXT_MODE_READ_CARD = ID_COMBOBOX_MELODY + NUMBER_MELODY + 1,
    ID_COMBOBOX_ANTIBREAK_SENS,                     // Чувствительность антикражи
    ID_COMBOBOX_OSDP_BAUDRATE,
    ID_COMBOBOX_NUMBER_PORT,

    ID_CHECKBUTTON_EXT_SET_ENABLE,                 // Мастер карта - Расширенные настройки - Вкл/Выкл
    ID_CHECKBUTTON_FACTORY_SETTINGS,               // Мастер-карта - Заводские настройки - Вкл/Выкл
    ID_CHECKBUTTON_OFFLINE_ENABLED,
    ID_CHECKBUTTON_ANTIBREAK_ENABLED,
    ID_CHECKBUTTON_OSDP_ENABLED,
    ID_CHECKBUTTON_COMPORT_AUTOFIND,
    ID_CHECKBUTTON_MODE_EKO,
    ID_CHECKBUTTON_ADVANCED_PROTECTION,             // Карта - Дополнительно - Улучшенная защита

    ID_CHECKBOX_EXT_PARITY,
    ID_CHECKBOX_EXT_INVERSION_CODE,
    ID_CHECKBOX_EXT_CONTROL_BIT,
    ID_CHECKBOX_RANGE,
    ID_CHECKBOX_WRITE_MASTER_PASSWORD,              // Запись на мастер карту - Пароля
    ID_CHECKBOX_WRITE_MASTER_CONFIG,                // Запись на мастер карту - Конфигураций
    ID_CHECKBOX_WRITE_CLIENT_PASSWORD,              // Карты доступа - Запись на карту - Пароля
    ID_CHECKBOX_WRITE_CLIENT_NEW_NUMBER,            // Карты доступа - Запись на карту - Нового номера карты
    ID_CHECKBOX_OFFLINE_SECURITY_MODE,              // Включение режима охраны
    ID_CHECKBOX_OSDP_ENCRYPTION,
    ID_CHECKBOX_DISABLE_BELOW_LEVEL_SL3,
    ID_CHECKBOX_WIEGAND_CONTROL_BITS,
    ID_CHECKBOX_WIEGAND_INVERSE_CONTROL_BITS,
    ID_CHECKBOX_WIEGAND_REVERSE_ORDER_BITS,
    ID_CHECKBOX_WIEGAND_NUID_DISCARD_LSB,
    ID_CHECKBOX_WIEGAND_FULL_GUID,

    ID_SLIDER_RED,
    ID_SLIDER_GREEN,
    ID_SLIDER_BLUE,
    ID_SLIDER_BRIGHTNESS,

    ID_SPINCNTRL_VOLUME,                            // Громкость

    ID_STATICBOX_NUMBERS_CARDS = ID_SPINCNTRL_VOLUME + NUMBER_MELODY + 1,

    ID_TEXTCNTRL_NUMBER_FIRST,
    ID_TEXTCNTRL_NUMBER_LAST,
    ID_TEXTCNTRL_NUMBER_NEXT,
    ID_TEXTCNTRL_PASSWORD_FACTORY,
    ID_TEXTCNTRL_PASSWORD_ONE,
    ID_TEXTCNTRL_PASSWORD_TWO,
    ID_TEXTCNTRL_KEY_CRYPTO_OSDP,
    ID_TEXTCNTRL_EXT_PERIOD_AUTOREPEAT,             // Переод автоповтора
    ID_TEXTCNTRL_OFFLINE_TIME_LOCK,                 // Время подачи сигнала на замок
    ID_TEXTCNTRL_OFFLINE_TIME_ALARM,                // Время подачи сигнала тревоги
    ID_TEXTCNTRL_ANTIBREAK_NUMBER,
    ID_TEXTCNTRL_OSDP_ADDRESS,
    ID_TEXTCNTRL_KEY_9000,
    ID_TEXTCNTRL_KEY_9001,
    ID_TEXTCNTRL_KEY_9002,
    ID_TEXTCNTRL_KEY_9003,
    ID_TEXTCNTRL_KEY_9004,
    ID_TEXTCNTRL_WIEGAND_VALUE,

    ID_STATICTEXT_PERIOD_AUTOREPEAT,                // Период автоповтора
    ID_STATICTEXT_MODE_READ_CARD,
    ID_STATICTEXT_TIME_LOCK,
    ID_STATICTEXT_TIME_ALARM,
    ID_STATICTEXT_ANTIBREAK_NUMBER,
    ID_STATICTEXT_ANTIBREAK_SENS,
    ID_STATICTEXT_ADDRESS_OSDP,
    ID_STATICTEXT_BAUDRATE_OSDP,
    ID_STATICTEXT_NUMBER_PORT,

    // Менюшки пошли
    ID_MENU_LIST_CARD_MARK_AS_GREEN,
    ID_MENU_LIST_CARD_MARK_AS_RED,
    ID_MENU_LIST_CARD_SELECT_ALL,
    ID_MENU_LIST_CARD_DELETE
};

class wxConfigBase;

extern wxConfigBase *g_config;
extern wxFileConfig *g_file_config;
extern wxSize BUTTON_SIZE;
extern wxSize BUTTON_SIZE_SMALL;
static const int BUTTON_HEIGHT = 25;
static const int TEXTCNTRL_HEIGHT = 21;
static const int BUTTON_WIDTH = 165;
static const int BUTTON_WIDTH_BIG = 200;
static const int SIZE_SPACE = 5;
static const int WIDTH_BOX = 185;


union BitSet16
{
    BitSet16(int16 value = 0) : sign_word(value) { }

    int16  sign_word;
    uint8  byte[2];

    void SwapBytes()
    {
        uint8 temp = byte[0];
        byte[0] = byte[1];
        byte[1] = temp;
    }
};


union BitSet32
{
    uint   word;
    uint16 half_word[2];
    uint8  bytes[4];

    static BitSet32 FromBytes(uint8 b0, uint8 b1, uint8 b2, uint8 b3)
    {
        BitSet32 bs = { (uint)(b0 | (b1 << 8) | (b2 << 16) | (b3 << 24)) };

        return bs;
    }

    static BitSet32 NTOH(uint8 b[4])
    {
        BitSet32 bs;

        bs.bytes[0] = b[3];
        bs.bytes[1] = b[2];
        bs.bytes[2] = b[1];
        bs.bytes[3] = b[0];

        return bs;
    }
};


union BitSet64
{
    BitSet64(uint64 _value = 0) : long_word(_value) {}
    uint64 long_word;
    uint   word[2];
    uint16 half_word[4];
    uint8  bytes[8];
};


union BitSet128
{
    BitSet128(uint64 _value_hi = 0, uint64 _value_low = 0)
    {
        word64[0] = _value_low;
        word64[1] = _value_hi;
    }
    uint64 word64[2];
    uint   word[4];
    uint16 half_word[8];
    uint8  bytes[16];

    wxString ToASCII() const;

    void FromASCII(const wxString &);

    bool operator==(const BitSet128 &) const;
    bool operator!=(const BitSet128 &) const;
};


#define _bitset(bits)                               \
  ((uint8)(                                         \
  (((uint8)((uint)bits / 01)        % 010) << 0) |  \
  (((uint8)((uint)bits / 010)       % 010) << 1) |  \
  (((uint8)((uint)bits / 0100)      % 010) << 2) |  \
  (((uint8)((uint)bits / 01000)     % 010) << 3) |  \
  (((uint8)((uint)bits / 010000)    % 010) << 4) |  \
  (((uint8)((uint)bits / 0100000)   % 010) << 5) |  \
  (((uint8)((uint)bits / 01000000)  % 010) << 6) |  \
  (((uint8)((uint)bits / 010000000) % 010) << 7)))

#define BINARY_U8( bits ) ((uint8)_bitset(0##bits))


static pString REVERSE_PROMT = "-> ";
static pString DIRECT_PROMT = "<- ";


#define _SET_BIT(value, bit) (value |= (1 << (bit)))
#define _GET_BIT(value, bit) ((value) & (1 << (bit)))


#define SAFE_DELETE(ptr)  do { delete ptr; ptr = nullptr; } while(0)
