// 2023/08/09 13:34:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/TextControls.h"
#include "Settings/Settings.h"


TextCtrlNumbers::TextCtrlNumbers(wxWindow *parent, int id, int value, const wxPoint &position, const wxSize &size) :
    wxTextCtrl(parent, id, wxString::Format("%d", value), position, size)
{
    Bind(wxEVT_CHAR, &TextCtrlNumbers::OnEventChar, this);
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


TextCtrlNumbersLimits::TextCtrlNumbersLimits(wxWindow *parent, int id, int value, int _min, int _max, const wxPoint &position, const wxSize &size) :
    TextCtrlNumbers(parent, id, value, position, size),
    min(_min),
    max(_max)
{
    Bind(wxEVT_CHAR, &TextCtrlNumbersLimits::OnEventChar, this);
}


int TextCtrlNumbersLimits::Max() const
{
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


TextCtrlNumber::TextCtrlNumber(wxWindow *parent, int id, const wxString &text_val, const wxPoint &pos, const wxSize &size, int _min, int _max) :
    wxTextCtrl(parent, id, text_val, pos, size),
    min(_min),
    max(_max)
{
    Bind(wxEVT_CHAR, &TextCtrlNumber::OnEventChar, this);
}


void TextCtrlNumber::OnEventChar(wxKeyEvent &event)
{
    int code = event.GetKeyCode();

    if (code == WXK_BACK || code == WXK_DELETE ||
        code == WXK_LEFT || code == WXK_RIGHT ||
        code == WXK_HOME || code == WXK_END ||
        code == WXK_TAB || code == WXK_RETURN)
    {
        event.Skip();
        return;
    }
    else if ((code >= '0' && code <= '9'))
    {
        wxString str = GetValue();
        str += (char)(code |= 0x30);

        long value = 0;

        str.ToLong(&value);

        if (value >= min && value <= max)
        {
            event.Skip();
            return;
        }
    }

    wxBell();
}
