// 2025/7/12 17:36:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel11_Menu/MenuDialog.h"


class DialogTable : public MenuDialog
{
public:

    DialogTable();

    static DialogTable *self;

private:

    const pchar BTN_SAVE = "Сохранить";
    const pchar BTN_DELETE = "Удалить";
    const pchar BTN_EXPAND = "Развернуть";
    const pchar BTN_COLLAPSE = "Свернуть";
};
