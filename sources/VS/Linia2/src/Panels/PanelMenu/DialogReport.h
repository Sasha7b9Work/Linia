// 2025/7/12 18:18:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelMenu/MenuDialog.h"


class DialogReport : public MenuDialog
{
public:

    DialogReport();

    static DialogReport *self;

private:

    static const pchar BTN_NEW;
    static const pchar BTN_SAVE;
    static const pchar BTN_DELETE;
    static const pchar BTN_EXPAND;
    static const pchar BTN_COLLAPSE;
    static const pchar BTN_CLOSE;
};
