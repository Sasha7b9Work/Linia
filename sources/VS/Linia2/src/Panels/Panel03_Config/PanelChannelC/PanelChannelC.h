// 2025/8/9 10:39:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/RadioButtonICO.h"


class PanelChannelC : public wxPanel
{
public:

    PanelChannelC(wxPanel *parent, int x, int w);

private:

    RadioButtonICO *rbScan[7];                   // Развёртка

    void OnEventRadioButton(wxCommandEvent &);
};