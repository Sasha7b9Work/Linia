// 2025/10/13 13:53:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageDebug/Notebook/PageChip.h"


class PageFPGA : public PageChip
{
public:

    PageFPGA(wxNotebook *);

    static PageFPGA *self;
};
