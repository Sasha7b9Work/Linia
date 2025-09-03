// 2025/08/30 11:36:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel12_Debug/PageChip.h"


class PageAD5531 : public PageChip
{
public:

    PageAD5531(wxNotebook *);

    static PageAD5531 *self;
};
