// 2025/6/1 17:50:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"


class PanelPercents : public Panel
{
public:

    static const int HEIGHT = 20;

    PanelPercents(wxWindow *parent);

    static PanelPercents *self;
};
