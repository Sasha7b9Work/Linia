// 2026/08/19 10:59:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageDebug/Notebook/PageChip.h"

/*
    Страница платы контроллера stm32
*/


class PageSTM32 : public PageChip
{
public:

    PageSTM32(wxNotebook *);

    static PageSTM32 *self;

private:

    void ProcessUpdate(pchar);
};
