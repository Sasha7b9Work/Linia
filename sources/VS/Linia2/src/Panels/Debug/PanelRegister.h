// 2025/6/4 10:25:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Визуальное представление регистра


class PanelRegister : public wxPanel
{
public:
    PanelRegister(wxWindow *parent, const wxString &title, int bit_depth);

private:

    int bit_depth = 0;
};
