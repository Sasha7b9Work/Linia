// 2025/10/07 12:24:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelDebug/PagesBlocks/PageChip.h"


class PageMeasCurrent : public PageChip
{
public:

    PageMeasCurrent(wxNotebook *);

    static PageMeasCurrent *self;
};

