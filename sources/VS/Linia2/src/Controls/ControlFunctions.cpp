// 2025/03/20 08:09:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/ControlFunctions.h"


wxBoxSizer *CF::CreateButton(wxWindow *parent, wxButton **button, int id, const wxSize &size, const wxString &label)
{
    wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL);

    *button = new wxButton(parent, id, label, wxDefaultPosition, size);

    box->AddSpacer(20);

    box->Add(*button, 0, wxALIGN_CENTER);

    return box;
}
