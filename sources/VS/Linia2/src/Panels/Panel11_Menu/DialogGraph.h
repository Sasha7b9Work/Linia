// 2025/7/12 17:21:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel11_Menu/MenuDialog.h"


class DialogGraph : public MenuDialog
{
public:

    DialogGraph();

    static DialogGraph *self;

private:

    static void OnButtonSizePoint();
    static void OnButtonStyleCurve();
    static void OnButtonColor();
    static void OnButtonMarkers();
    static void OnButtonScale();
    static void OnButtonSave();
    static void OnButtonReset();
};


class DialogGraphColor : public MenuDialog
{
public:

    DialogGraphColor();

    static DialogGraphColor *self;

private:

    static void OnButtonBackground();
    static void OnButtonGrid();
    static void OnButtonFont();
    static void OnButtonCurve();
    static void OnButtonLink();
    static void OnButtonSecant();   // Секущая

    static bool SelectColor(const wxString &title, wxColour &);
};
