// 2025/6/1 18:17:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"


class PanelScale : public Panel
{
public:

    PanelScale(wxWindow *parent);

    static PanelScale *self;
};
