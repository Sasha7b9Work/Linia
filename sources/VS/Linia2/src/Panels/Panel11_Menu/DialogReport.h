// 2025/7/12 18:18:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel11_Menu/MenuDialog.h"


class DialogReport : public MenuDialog
{
public:

    DialogReport();

    static DialogReport *self;

private:

    const pchar BTN_NEW = "Новый";
    const pchar BTN_SAVE = "Сохранить";
    const pchar BTN_DELETE = "Удалить";
    const pchar BTN_EXPAND = "Развернуть";
    const pchar BTN_COLLAPSE = "Свернуть";
    const pchar BTN_CLOSE = "Закрыть";
};
