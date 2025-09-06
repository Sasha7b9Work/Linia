// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define VERSION_BUILD 90
#define DATE_BUILD "2025-09-06 13:32:48"


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
#include <wx/popupwin.h>
#include <wx/scrolwin.h>
#include <wx/combo.h>
#include <wx/valnum.h>
#include <wx/datetime.h>
#include <wx/dirctrl.h>
#include <wx/dir.h>
#include <map>
#include <queue>
#include <mutex>
#include <stack>
#include <thread>
#pragma warning(pop)


#include <cstdint>


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

// Яркость элементов управления
#ifdef WIN32
    #define LIGHTNESS 300
#else
    #define LIGHTNESS 140
#endif

