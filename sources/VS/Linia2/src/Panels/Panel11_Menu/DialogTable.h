// 2025/7/12 17:36:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel11_Menu/MenuDialog.h"


class DialogTable : public MenuDialog
{
public:

    DialogTable();

    static DialogTable *self;

private:

    static void OnButtonSave();
    static void OnButtonDelete();
    static void OnButtonExpand();
    static void OnButtonCollapse();
};
