// 2025/7/27 21:36:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Dialog.h"


class WindowTableOffsets : public Dialog
{
public:

    static const int WIDTH = 500;
    static const int HEIGHT = 500;

    WindowTableOffsets();

private:

    void OnEventButton(wxCommandEvent &);
    void OnEventRadioButton(wxCommandEvent &);
};
