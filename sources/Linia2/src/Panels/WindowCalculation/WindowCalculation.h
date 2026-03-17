// 2026/03/16 09:12:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/DraggedWindow.h"


class WindowCalculation : public DraggedDialog
{
public:
    WindowCalculation();

private:

    void CreateTitleBar(wxWindow *, wxBoxSizer *);

    void CreateMainPanel2();

    wxPanel *titleBar = nullptr;
};
