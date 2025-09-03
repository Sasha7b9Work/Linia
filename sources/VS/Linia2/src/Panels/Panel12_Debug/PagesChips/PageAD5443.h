// 2025/08/30 11:23:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel12_Debug/PagesChips/PageChip.h"


class PageAD5443 : public PageChip
{
public:

    PageAD5443(wxNotebook *);

    static PageAD5443 *self;
};
