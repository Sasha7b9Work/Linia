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
    ID_DEFINES_COUNT
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


enum
{
    ID_BTN_MENU_FILE_NEW = wxID_HIGHEST,
    ID_BTN_MENU_FILE_OPEN,
    ID_BTN_MENU_FILE_CLOSE,
    ID_BTN_MENU_FILE_SAVE,
    ID_BTN_MENU_FILE_SAVE_AS,
    ID_BTN_MENU_FILE_DELETE
};
