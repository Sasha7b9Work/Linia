// 2023/08/09 13:34:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"


class TextCtrlNumbers : public wxTextCtrl
{
public:

    TextCtrlNumbers(wxWindow *parent, int id, int value, const wxPoint & = wxDefaultPosition, const wxSize & = wxDefaultSize);

protected:

    void OnEventChar(wxKeyEvent &);
};


class TextCtrlNumbersLimits : public TextCtrlNumbers
{
public:
    TextCtrlNumbersLimits(wxWindow *parent, int id, int value, int min, int max, const wxPoint & = wxDefaultPosition, const wxSize & = wxDefaultSize);

    // Ограничить значение в соответствии с установками
    void LimitValue();

private:
    int min = 0;
    int max = 0;

    int Max() const;

protected:

    void OnEventChar(wxKeyEvent &);
};


class TextCtrlPassword : public TextCtrlNumbers
{
public:

    TextCtrlPassword(wxWindow *parent, int id, const wxPoint &position = wxDefaultPosition);

    void SetUInt64(uint64);
};
