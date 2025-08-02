// 2025/6/1 17:31:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"


class PanelButtons : public Panel
{
public:

    static const int HEIGHT = 35;

    PanelButtons(wxWindow *parent);

    static PanelButtons *self;

private:

    wxButton *btnFile = nullptr,
        *btnTest = nullptr,
        *btnSetup = nullptr,
        *btnArchiv = nullptr,
        *btnGraph = nullptr,
        *btnTable = nullptr,
        *btnReport = nullptr,
        *btnMeasure = nullptr,
        *btnDebug = nullptr;

    void OnEventButton(wxCommandEvent &);

    int NumButtons() const;
};
