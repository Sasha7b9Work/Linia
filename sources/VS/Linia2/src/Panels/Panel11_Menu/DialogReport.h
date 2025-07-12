// 2025/7/12 18:18:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel11_Menu/MenuDialog.h"


class DialogReport : public MenuDialog
{
public:

    DialogReport();

    static DialogReport *self;

private:

    static void OnButtonNew();
    static void OnButtonSave();
    static void OnButtonDelete();
    static void OnButtonExpand();
    static void OnButtonCollapse();
    static void OnButtonClose();
};
