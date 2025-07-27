// 2025/7/27 13:41:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Dialog.h"


class WindowCorretionZero : public Dialog
{
public:

    static const int WIDTH = 300;
    static const int HEIGHT = 500;

    WindowCorretionZero();

private:

    void OnEventRadioButton(wxCommandEvent &);


};
