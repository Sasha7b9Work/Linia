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

    void OnEventButton(wxCommandEvent &);

    int NumButtons() const;
};
