// 2025/6/1 17:31:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"


class PanelMenu : public Panel
{
public:

    static const int HEIGHT = 35;

    PanelMenu(wxWindow *parent);

    static PanelMenu *self;

private:

    wxButton *btnModels = nullptr,
        *btnTests = nullptr,
        *btnSetup = nullptr,
        *btnArchiv = nullptr,
        *btnTable = nullptr,
        *btnReport = nullptr,
        *btnCalculation = nullptr,
        *btnDebug = nullptr;

    void OnEventButton(wxCommandEvent &);

    int NumButtons() const;
};
