// 2025/6/1 17:31:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"


class PanelButtons : public Panel
{
public:

    static const int HEIGHT = 35;

    PanelButtons(wxWindow *parent);

private:

    static const int NUM_BUTTONS = 8;

    wxButton *btnFile = nullptr;
    wxButton *btnTest = nullptr;
    wxButton *btnSettings = nullptr;
    wxButton *btnArchive = nullptr;
    wxButton *btnGraph = nullptr;
    wxButton *btnTable = nullptr;
    wxButton *btnReport = nullptr;
    wxButton *btnMeasure = nullptr;
};
