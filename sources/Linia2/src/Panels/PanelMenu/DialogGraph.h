// 2025/7/12 17:21:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelMenu/MenuDialog.h"


class DialogGraph : public MenuDialog
{
public:

    DialogGraph();

    static DialogGraph *self;

private:

    static const pchar BTN_STYLE_CURVE;
    static const pchar BTN_MARKERS;
    static const pchar BTN_SCALE;
    static const pchar BTN_SAVE;
    static const pchar BTN_RESET;
};
