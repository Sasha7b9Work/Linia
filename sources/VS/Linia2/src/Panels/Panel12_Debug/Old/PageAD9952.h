// 2025/6/3 22:44:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel12_Debug/PagesChips/PageChip.h"


class PageAD9952 : public PageChip
{
public:

    PageAD9952(wxNotebook *);

    static PageAD9952 *self;
};
