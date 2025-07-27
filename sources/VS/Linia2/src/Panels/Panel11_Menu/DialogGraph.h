// 2025/7/12 17:21:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel11_Menu/MenuDialog.h"


class DialogGraph : public MenuDialog
{
public:

    DialogGraph();

    static DialogGraph *self;
};


class DialogGraphColor : public MenuDialog
{
public:

    DialogGraphColor();

    static DialogGraphColor *self;

    static bool SelectColor(const wxString &title, wxColour &);
};
