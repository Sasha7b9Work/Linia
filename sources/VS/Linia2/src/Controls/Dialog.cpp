// 2025/7/25 12:23:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Dialog.h"
#include "Utils/SystemDepend.h"


int Dialog::CreateLabelGroup(int x, int y, const wxString &label)
{
    new wxStaticLine(this, wxID_ANY, { x, SD::Y_SB(y) }, { 100, -1 }, wxLI_HORIZONTAL);

    y += 7;

    new wxStaticText(this, wxID_ANY, label, { 20, SD::Y_SB(y) });

    return y + 25;
}


int Dialog::CreateRadioButton(int x, int y, int id, const wxString &label)
{
    new wxRadioButton(this, id, label, { x, SD::Y_SB(y) });

    return y + 20;
}
