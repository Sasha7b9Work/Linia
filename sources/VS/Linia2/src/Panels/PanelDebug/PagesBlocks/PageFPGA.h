// 2025/10/13 13:53:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelDebug/PagesBlocks/PageChip.h"


class PageFPGA : public PageChip
{
public:

    PageFPGA(wxNotebook *);

    static PageFPGA *self;

    void Pack();
    void Unpack();
};
