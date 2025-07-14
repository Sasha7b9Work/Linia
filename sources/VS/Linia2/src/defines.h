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
static const int BUTTON_HEIGHT = 25;
static const int TEXTCNTRL_HEIGHT = 21;
static const int BUTTON_WIDTH = 165;

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
    ID_TIMER = wxID_HIGHEST + 1,
    ID_LINE,
    ID_DEFINES_COUNT,

    ID_BTN_MENU_FILE_NEW,
    ID_BTN_MENU_FILE_OPEN,
    ID_BTN_MENU_FILE_CLOSE,
    ID_BTN_MENU_FILE_SAVE,
    ID_BTN_MENU_FILE_SAVE_AS,
    ID_BTN_MENU_FILE_DELETE,

    ID_BTN_MENU_TEST_APPEND,
    ID_BTN_MENU_TEST_APPEND_ACTIVE,
    ID_BTN_MENU_TEST_APPEND_FROM_LIBRARY,
    ID_BTN_MENU_TEST_SAVE_PARAMETERS,
    ID_BTN_MENU_TEST_DELETE,
    ID_BTN_MENU_TEST_DELETE_ALL,
    ID_BTN_MENU_TEST_COPY,
    ID_BTN_MENU_TEST_INSERT,
    ID_BTN_MENU_TEST_RENAME,
    ID_BTN_MENU_TEST_LIBRARY_TESTS,
    ID_BTN_MENU_TEST_SAVE_TO_LIBRARY,

    ID_BTN_MENU_SETUP_USER,
    ID_BTN_MENU_SETUP_START,
    ID_BTN_MENU_SETUP_AUTOSAVE,
    ID_BTN_MENU_SETUP_CORRECTION_ZERO,
    ID_BTN_MENU_SETUP_CALIBRATION,
    ID_BTN_MENU_SETUP_TABLE,
    ID_BTN_MENU_SETUP_PASSWORD,
    ID_BTN_MENU_SETUP_ADDRESS_IP,
    ID_BTN_MENU_SETUP_MY_COMPUTER,

    ID_BTN_MENU_ARCHIVE_CLEAR,
    ID_BTN_MENU_ARCHIVE_DELETE_FROM,
    ID_BTN_MENU_ARCHIVE_LOAD_FROM,
    ID_BTN_MENU_ARCHIVE_OPEN,
    ID_BTN_MENU_ARCHIVE_CLOSE,

    ID_BTN_MENU_GRAPH_SIZE_POINT,
    ID_BTN_MENU_GRAPH_STYLE_CURVE,
    ID_BTN_MENU_GRAPH_COLOR,
    ID_BTN_MENU_GRAPH_COLOR_BACKGROUND,
    ID_BTN_MENU_GRAPH_COLOR_GRID,
    ID_BTN_MENU_GRAPH_COLOR_FONT,
    ID_BTN_MENU_GRAPH_COLOR_CURVE,
    ID_BTN_MENU_GRAPH_COLOR_LINK,
    ID_BTN_MENU_GRAPH_COLOR_SECANT,
    ID_BTN_MENU_GRAPH_MARKERS,
    ID_BTN_MENU_GRAPH_SCALE,
    ID_BTN_MENU_GRAPH_SAVE,
    ID_BTN_MENU_GRAPH_RESET,

    ID_BTN_MENU_TABLE_SAVE,
    ID_BTN_MENU_TABLE_DELETE,
    ID_BTN_MENU_TABLE_EXPAND,
    ID_BTN_MENU_TABLE_COLLAPSE,

    ID_BTN_MENU_REPORT_NEW,
    ID_BTN_MENU_REPORT_SAVE,
    ID_BTN_MENU_REPORT_DELETE,
    ID_BTN_MENU_REPORT_EXPAND,
    ID_BTN_MENU_REPORT_COLLAPSE,
    ID_BTN_MENU_REPORT_CLOSE,

    ID_PAN3_BTN_CHANNEL_C,
    ID_PAN3_BTN_CHANNEL_B,
    ID_PAN3_BTN_CHANNEL_S,
    ID_PAN3_BTN_SCHEME,
    ID_PAN3_BTN_CALCULATION,

    ID_PAN3_CHAN_C_RB_SCAN_1,
    ID_PAN3_CHAN_C_RB_SCAN_2,
    ID_PAN3_CHAN_C_RB_SCAN_3,
    ID_PAN3_CHAN_C_RB_SCAN_4,
    ID_PAN3_CHAN_C_RB_SCAN_5,
    ID_PAN3_CHAN_C_RB_SCAN_6,
    ID_PAN3_CHAN_C_RB_SCAN_7,
    ID_PAN3_CHAN_C_CHECK_DUTY_CYCLE,
    ID_PAN3_CHAN_C_COMBO_Uc,
    ID_PAN3_CHAN_C_COMBO_Ic,
    ID_PAN3_CHAN_C_COMBO_SOURCE,
    ID_PAN3_CHAN_C_SPIN_START,
    ID_PAN3_CHAN_C_SPIN_STOP,

    // Канал B
    // Генератор ступенек
    ID_PAN3_CHAN_B_GEN_COMBO_TYPE,
    ID_PAN3_CHAN_B_GEN_COMBO_IMPULSE,
    ID_PAN3_CHAN_B_GEN_COMBO_AMPLITUDE,
    ID_PAN3_CHAN_B_GEN_CHECK_SMALL,
    ID_PAN3_CHAN_B_GEN_SPIN_NUMBER_STEPS,
    ID_PAN3_CHAN_B_GEN_COMBO_POLARITY,
    // Смещение
    ID_PAN3_CHAN_B_GEN_OFFSET_SLIDER,
    ID_PAN3_CHAN_B_GEN_OFFSET_COMBO_POLARITY,
    // Ограничение
    ID_PAN3_CHAN_B_LIM_COMBO_RANGE,
    ID_PAN3_CHAN_B_LIM_SLIDER,

    // Канал S
    // Генератор ступенек
    ID_PAN3_CHAN_S_GEN_COMBO_TYPE,
    ID_PAN3_CHAN_S_GEN_COMBO_IMPULSE,
    ID_PAN3_CHAN_S_GEN_COMBO_AMPLITUDE,
    ID_PAN3_CHAN_S_GEN_SPIN_NUMBER_STEPS,
    ID_PAN3_CHAN_S_GEN_COMBO_POLARITY,
    // Смещение
    ID_PAN3_CHAN_S_GEN_OFFSET_SLIDER,
    ID_PAN3_CHAN_S_GEN_OFFSET_COMBO_POLARITY,
    // Ограничение
    ID_PAN3_CHAN_S_LIM_COMBO_RANGE,
    ID_PAN3_CHAN_S_LIM_SLIDER,

    // Расчёт
    // График
    ID_PAN5_GRAPH_LINE_X,
    ID_PAN5_GRAPH_LINE_Y,
    // Параметр
    ID_PAN5_PAR_LINE_FIND,
    ID_PAN5_PAR_LINE_FOR,
    ID_PAN5_PAR_LINE_VALUE_1,
    ID_PAN5_PAR_LINE_VALUE_2,
    ID_PAN5_PAR_RB_DY_DX,
    ID_PAN5_PAR_RB_DX_DY,
    // Пороговые значения
    ID_PAN5_BORDER_MIN,
    ID_PAN5_BORDER_MAX,

    ID_MENU_GRAPH_RESET,
    ID_MENU_GRAPH_FULL_SCREEN,
    ID_MENU_GRAPH_TRACK_X,
    ID_MENU_GRAPH_TRACK_Y,
    ID_MENU_GRAPH_TRACK_NONE
};
