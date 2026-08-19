// 2026/08/19 10:59:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageDebug/Notebook/PageChip.h"

/*
    Страница платы контроллера stm32
*/


class PageController : public PageChip
{
public:

    PageController(wxNotebook *);

    static PageController *self;
};
