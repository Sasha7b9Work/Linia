// 2025/6/1 17:13:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"


class PanelModeUpper : public Panel
{
public:
    PanelModeUpper(wxWindow* parent);

    static PanelModeUpper *self;
};
