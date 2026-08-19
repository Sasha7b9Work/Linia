// 2025/10/07 11:52:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageDebug/Notebook/PageChip.h"


/*

    Канал C - Формирователь

*/

class PageChannelForm : public PageChip
{
public:

    PageChannelForm(wxNotebook *);

    static PageChannelForm *self;
};
