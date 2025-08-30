// 2025/08/30 11:37:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel12_Debug/PageChip.h"


class PageAD7691 : public PageChip
{
public:

    PageAD7691(wxNotebook *);

    static PageAD7691 *self;
};
