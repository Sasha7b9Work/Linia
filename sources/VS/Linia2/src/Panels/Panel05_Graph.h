// 2025/6/1 17:45:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"


class PanelGraph : public Panel
{
public:

    static const int HEIGHT = 500;

    PanelGraph(wxWindow *parent);

    static PanelGraph *self;
};
