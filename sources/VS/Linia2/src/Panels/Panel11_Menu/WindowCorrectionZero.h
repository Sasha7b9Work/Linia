// 2025/7/27 13:41:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Dialog.h"


class WindowCorretionZero : public Dialog
{
public:

    static const int WIDTH = 350;
    static const int HEIGHT = 500;

    WindowCorretionZero();

private:

    wxVector <wxRadioButton *> buttons;

    void OnEventRadioButton(wxCommandEvent &);

    int CreateRadioButton(int x, int y, int id, const wxString &);

    int CreateLabelGroup(int x, int y, const wxString &);
};
