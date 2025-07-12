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
    ID_BTN_MENU_FILE_DELETE
};


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
