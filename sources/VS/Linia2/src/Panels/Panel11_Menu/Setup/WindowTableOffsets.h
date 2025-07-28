// 2025/7/27 21:36:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Dialog.h"
#include "Panels/Panel11_Menu/Setup/TableValues.h"


class WindowTableOffsets : public Dialog
{
public:

    static const int WIDTH = 660;
    static const int HEIGHT = 550;

    WindowTableOffsets();

private:

    TableValues *table = nullptr;

    void FillTable();

    void OnEventButton(wxCommandEvent &);
    void OnEventRadioButton(wxCommandEvent &);
};
