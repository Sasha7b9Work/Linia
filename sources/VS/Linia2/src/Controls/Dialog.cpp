// 2025/7/25 12:23:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Dialog.h"
#include "Utils/SystemDepend.h"


int Dialog::CreateLabelGroup(wxWindow *parent, int x, int y, const wxString &label)
{
    y += 15;

    new wxStaticLine(parent, wxID_ANY, { x, SD::Y_SB(y) }, { 100, -1 }, wxLI_HORIZONTAL);

    y += 7;

    new wxStaticText(parent, wxID_ANY, label, { x + 10, SD::Y_SB(y) } );

    return y + 25;
}


int Dialog::CreateRadioButton(wxWindow *parent, int x, int y, wxRadioButton **rb, const wxString &label)
{
    *rb = new wxRadioButton(parent, wxID_ANY, label, { x, SD::Y_SB(y) });

    return y + 20;
}
