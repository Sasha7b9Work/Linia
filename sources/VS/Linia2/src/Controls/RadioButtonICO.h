// 2025/08/08 21:54:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/PainterBMP.h"


class RadioButtonICO : public wxPanel
{
public:

    RadioButtonICO(wxWindow *, const wxPoint &, const wxSize &, const wxString &name_ico);

    void SetValue(bool);

private:

    wxRadioButton *button = nullptr;
    PainterBMP *bmp = nullptr;

    void OnEventRadioButton(wxCommandEvent &);
    void OnEventMouseDown(wxMouseEvent &);

    void SendEvent();
};
