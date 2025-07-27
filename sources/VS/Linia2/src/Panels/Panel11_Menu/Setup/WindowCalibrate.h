// 2025/7/27 19:54:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Dialog.h"


class WindowCalibrate : public Dialog
{
public:

    static const int WIDTH = 450;
    static const int HEIGHT = 600;

    WindowCalibrate();

private:

    wxStaticText *textSerialNumber = nullptr;
    wxStaticText *textDateTime = nullptr;
};
