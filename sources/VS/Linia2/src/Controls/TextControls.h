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


// Ввод hex-битного значения до 128 бит длиной
class TextCtrlKeyHex128 : public wxTextCtrl
{
public:

    TextCtrlKeyHex128(wxWindow *parent, int id, const Key128 &, const wxPoint &position);

    Key128 key;                     // Здесь хранится текущее значение
    const Key128 start_key;         // Здесь хранится значение, переданное в конструкторе, чтобы можно было восстановить

    // true, если поле ввода полностью заполнено (32 символа)
    bool IsFieldCompletelyFilled() const;

    // Рассчитывает новое значение key (после нажатия клавиши)
    void CalculateKey();

protected:

    void OnEventChar(wxKeyEvent &);

    void SetKey128(const Key128 &);
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
