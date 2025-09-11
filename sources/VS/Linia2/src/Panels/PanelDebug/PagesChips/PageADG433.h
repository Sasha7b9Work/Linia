// 2025/08/30 11:25:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelDebug/PageChip.h"


class PageADG433 : public PageChip
{
public:

    PageADG433(wxNotebook *);

    static PageADG433 *self;
};
