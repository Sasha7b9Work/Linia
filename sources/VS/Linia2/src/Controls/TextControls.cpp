// 2023/08/09 13:34:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/TextControls.h"
#include "Panels/Notebook/PageUser/PageUser.h"
#include "Settings/Settings.h"


TextCtrlNumbers::TextCtrlNumbers(wxWindow *parent, int id, int value, const wxPoint &position, const wxSize &size) :
    wxTextCtrl(parent, id, wxString::Format("%d", value), position, size)
{
    Bind(wxEVT_CHAR, &TextCtrlNumbers::OnEventChar, this);
}


TextCtrlKeyHex128::TextCtrlKeyHex128(wxWindow *parent, int id, const Key128 &_key, const wxPoint &position) :
    wxTextCtrl(parent, id, "", position),
    key(_key),
    start_key(_key)
{
    wxSize size{ 205, TEXTCNTRL_HEIGHT };

    wxTextCtrl::SetMinSize(size);
    wxTextCtrl::SetMaxSize(size);
    wxTextCtrl::SetSize(size);

    wxTextCtrl::SetMaxLength(32);

    Bind(wxEVT_CHAR, &TextCtrlKeyHex128::OnEventChar, this);

    SetKey128(key);
}


void TextCtrlNumbers::OnEventChar(wxKeyEvent &event)
{
    if (event.GetId() == GetId())
//        GetStringSelection() == "")             // Если выбран текст, то возможно введение несанкционированных значений
    {
        int code = event.GetKeyCode();

        if ((code >= '0' && code <= '9') ||   // Цифры
            code == WXK_LEFT ||
            code == WXK_RIGHT ||
            code == WXK_UP ||
            code == WXK_DOWN ||
            code == WXK_HOME ||
            code == WXK_END ||
            code == WXK_RETURN ||
            code == WXK_BACK ||
            code == WXK_DELETE ||
            code == WXK_INSERT ||
            code == WXK_ESCAPE
            )
        {
            event.Skip();
        }
    }
}


void TextCtrlKeyHex128::OnEventChar(wxKeyEvent &event)
{
    if (event.GetId() == GetId())
    {
        int code = event.GetKeyCode();

        if ((code >= '0' && code <= '9') ||   // Цифры
            (code >= 'a' && code <= 'f') ||
            (code >= 'A' && code <= 'F') ||
            code == WXK_LEFT ||
            code == WXK_RIGHT ||
            code == WXK_UP ||
            code == WXK_DOWN ||
            code == WXK_HOME ||
            code == WXK_END ||
            code == WXK_RETURN ||
            code == WXK_BACK ||
            code == WXK_DELETE ||
            code == WXK_INSERT ||
            code == WXK_ESCAPE
            )
        {
            event.Skip();
        }
    }
}


void TextCtrlKeyHex128::SetKey128(const Key128 &_key)
{
    wxTextCtrl::SetValue(_key.bitset.ToASCII());
}


void TextCtrlKeyHex128::CalculateKey()
{
    key.bitset.FromASCII(GetValue());
}


bool TextCtrlKeyHex128::IsFieldCompletelyFilled() const
{
    return GetValue().Length() == 128 / 4;
}


TextCtrlNumbersLimits::TextCtrlNumbersLimits(wxWindow *parent, int id, int value, int _min, int _max, const wxPoint &position, const wxSize &size) :
    TextCtrlNumbers(parent, id, value, position, size),
    min(_min),
    max(_max)
{
    Bind(wxEVT_CHAR, &TextCtrlNumbersLimits::OnEventChar, this);
}


int TextCtrlNumbersLimits::Max() const
{
    if (GetParent()->GetParent() == PageUser::self && SET::OFFLINE::enabled.Get())
    {
        return 600;
    }

    return max;
}


void TextCtrlNumbersLimits::OnEventChar(wxKeyEvent &event)
{
    if (event.GetId() == GetId())
    {
        int code = event.GetKeyCode();

        if (code >= 0x30 && code <= 0x39)
        {
            int value = wxAtoi(GetValue()) * 10 + (code & 0x0f);

            if (value >= min && value <= Max())
            {
                event.Skip();
            }
            else
            {
                return;
            }
        }
    }

    event.Skip();
}


void TextCtrlNumbersLimits::LimitValue()
{
    int value = wxAtoi(GetValue());

    if (value < min)
    {
        SetValue(wxString::Format("%d", min));
    }

    if (value > Max())
    {
        SetValue(wxString::Format("%d", Max()));
    }
}


TextCtrlPassword::TextCtrlPassword(wxWindow *parent, int id, const wxPoint &position) : TextCtrlNumbers(parent, id, 0, position)
{
    wxSize size{ 97, TEXTCNTRL_HEIGHT };

    wxTextCtrl::SetMinSize(size);
    wxTextCtrl::SetMaxSize(size);
    wxTextCtrl::SetSize(size);

    wxTextCtrl::SetMaxLength(14);
}


void TextCtrlPassword::SetUInt64(uint64 value)
{
    SetLabelText(wxString::Format("%llu", value));
}
