// 2025/6/1 18:08:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"


class PanelTable : public Panel
{
public:

    PanelTable(wxWindow *parent);

    static PanelTable *self;
};
