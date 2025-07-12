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
    ID_BTN_MENU_REPORT_CLOSE
};
