// 2025/7/12 17:21:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelMenu/MenuDialog.h"


class DialogGraph : public MenuDialog
{
public:

    DialogGraph();

    static DialogGraph *self;

private:

    static const pchar BTN_SIZE_POINT;
    static const pchar BTN_STYLE_CURVE;
    static const pchar BTN_COLOR;
    static const pchar BTN_MARKERS;
    static const pchar BTN_SCALE;
    static const pchar BTN_SAVE;
    static const pchar BTN_RESET;
};


class DialogGraphColor : public MenuDialog
{
public:

    DialogGraphColor();

    static DialogGraphColor *self;

    static bool SelectColor(const wxString &title, wxColour &);

private:

    static const pchar BTN_GRID;
    static const pchar BTN_FONT;
    static const pchar BTN_CURVE;
    static const pchar BTN_LINK;
    static const pchar BTN_SECANT;
};
