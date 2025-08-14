// 2025/7/23 14:18:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
*   Собственный спинбокс. Разработанн ввиду того, что стандартный на Linux очень большой
*/


class SpinBox : public wxPanel
{
public:

    SpinBox(wxWindow *parent, const wxPoint &, const wxSize &, int min, int max);

private:

    int min = 0;
    int max = 0;

    wxTextCtrl *text = nullptr;
};
