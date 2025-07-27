// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

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
#include <wx/graphics.h>
#include <wx/colordlg.h>
#include <wx/aui/aui.h>
#include <wx/msgdlg.h>
#include <wx/grid.h>
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


extern wxConfigBase *g_config;
extern wxFileConfig *g_file_config;

static const int BUTTON_WIDTH = 165;
static const int BUTTON_HEIGHT = 25;
static const int TEXTCNTRL_HEIGHT = 21;

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

static pString REVERSE_PROMT = "-> ";
static pString DIRECT_PROMT = "<- ";

#define SAFE_DELETE(ptr)  do { delete ptr; ptr = nullptr; } while(0)


enum
{
    ID_BTN_RETURN_TO_MAIN_PAGE = wxID_HIGHEST + 1,
    ID_MENU_SCALE,
    ID_BTN_START,
    ID_TIMER_APP,
    ID_TIMER_TIME,

    /////////////////////////////////////////////////////////////////////////////////// Панель 3 - Конфигурация
    ID_BTN_CHANNEL_C,
    ID_BTN_CHANNEL_B,
    ID_BTN_CHANNEL_S,
    ID_BTN_SCHEME,
    ID_BTN_CALCULATION,

    ID_CHAN_C_RB_SCAN_1,
    ID_CHAN_C_RB_SCAN_2,
    ID_CHAN_C_RB_SCAN_3,
    ID_CHAN_C_RB_SCAN_4,
    ID_CHAN_C_RB_SCAN_5,
    ID_CHAN_C_RB_SCAN_6,
    ID_CHAN_C_RB_SCAN_7,
    ID_CHAN_C_CHECK_DUTY_CYCLE,
    ID_CHAN_C_COMBO_Uc,
    ID_CHAN_C_COMBO_Ic,
    ID_CHAN_C_COMBO_SOURCE,
    ID_CHAN_C_SPIN_START,
    ID_CHAN_C_SPIN_STOP,

        // Схема включения
    ID_PAN3_TEST_COMBOBOX,
    ID_PAN3_TEST_BUTTON,
    ID_COMM_COMBO_TYPE,
    ID_COMM_COMBO_CHAN_C,
    ID_COMM_COMBO_CHAN_B,
    ID_COMM_COMBO_C,
    ID_COMM_COMBO_B,
    ID_COMM_COMBO_E,

        // Канал B
        // Генератор ступенек
    ID_CHAN_B_GEN_COMBO_TYPE,
    ID_CHAN_B_GEN_COMBO_IMPULSE,
    ID_CHAN_B_GEN_COMBO_AMPLITUDE,
    ID_CHAN_B_GEN_CHECK_SMALL,
    ID_CHAN_B_GEN_SPIN_NUMBER_STEPS,
    ID_CHAN_B_GEN_COMBO_POLARITY,
        // Смещение
    ID_CHAN_B_GEN_OFFSET_SLIDER,
    ID_CHAN_B_GEN_OFFSET_COMBO_POLARITY,
        // Ограничение
    ID_CHAN_B_LIM_COMBO_RANGE,
    ID_CHAN_B_LIM_SLIDER,

        // Канал S
        // Генератор ступенек
    ID_CHAN_S_GEN_COMBO_TYPE,
    ID_CHAN_S_GEN_COMBO_IMPULSE,
    ID_CHAN_S_GEN_COMBO_AMPLITUDE,
    ID_CHAN_S_GEN_SPIN_NUMBER_STEPS,
    ID_CHAN_S_GEN_COMBO_POLARITY,
        // Смещение
    ID_CHAN_S_GEN_OFFSET_SLIDER,
    ID_CHAN_S_GEN_OFFSET_COMBO_POLARITY,
        // Ограничение
    ID_CHAN_S_LIM_COMBO_RANGE,
    ID_CHAN_S_LIM_SLIDER,

        // Расчёт
        // График
    ID_GRAPH_LINE_X,
    ID_GRAPH_LINE_Y,
        // Параметр
    ID_PAR_LINE_FIND,
    ID_PAR_LINE_FOR,
    ID_PAR_LINE_VALUE_1,
    ID_PAR_LINE_VALUE_2,
    ID_PAR_RB_DY_DX,
    ID_PAR_RB_DX_DY,
        // Пороговые значения
    ID_BORDER_MIN,
    ID_BORDER_MAX,

    /////////////////////////////////////////////////////////////////////////////////// Панель 4
    ID_PAN4_TEXT_NAME,

    /////////////////////////////////////////////////////////////////////////////////// Панель 8
    ID_PAN8_COMBO,

    // Настройки
    ID_BTN_USER,
    ID_BTN_AUTOSAVE,
    ID_BTN_CORRECTION_ZERO,
    ID_BTN_CALIBRATION,
    ID_BTN_TABLE,
    ID_BTN_PASSWORD,
    ID_BTN_ADDRESS_IP,
    ID_BTN_MY_COMPUTER,

    // Настройки - установка коррекции нуля
    ID_RB_CHAN_C_MEAS_I,
    ID_RB_CHAN_C_MEAS_U,
    ID_RB_CHAN_B_MEAS_I,
    ID_RB_CHAN_B_MEAS_U,
    ID_RB_CHAN_B_SOURCE_I,
    ID_RB_CHAN_B_SOURCE_U,
    ID_RB_CHAN_S_MEAS_I,
    ID_RB_CHAN_S_MEAS_U,
    ID_RB_CHAN_S_SOURCE_I,
    ID_RB_CHAN_S_SOURCE_U,
    ID_BTN_CANCEL,

    // Библиотека пользовательских тестов
    ID_COMBO,
    ID_BTN_LOAD,
    ID_BTN_DELETE,
    LD_BTN_DELETE_ALL
};
