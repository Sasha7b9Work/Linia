// 2025/7/12 17:36:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelMenu/MenuDialog.h"


class DialogTable : public MenuDialog
{
public:

    DialogTable();

    static DialogTable *self;

private:

    static const pchar BTN_SAVE;
    static const pchar BTN_DELETE;
    static const pchar BTN_EXPAND;
    static const pchar BTN_COLLAPSE;
};
