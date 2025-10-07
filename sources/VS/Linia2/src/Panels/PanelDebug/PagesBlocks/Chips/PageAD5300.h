// 2025/08/30 11:33:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelDebug/PagesBlocks/PageChip.h"


class PageAD5300 : public PageChip
{
public:

    PageAD5300(wxNotebook *);

    static PageAD5300 *self;
};

