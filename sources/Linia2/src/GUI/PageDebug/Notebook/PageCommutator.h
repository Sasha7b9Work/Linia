// 2025/10/07 11:47:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageDebug/Notebook/PageChip.h"


class PageCommutator : public PageChip
{
public:

    PageCommutator(wxNotebook *);

    static PageCommutator *self;
};
