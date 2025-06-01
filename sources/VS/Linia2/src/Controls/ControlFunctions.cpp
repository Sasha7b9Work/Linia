// 2025/03/20 08:09:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/ControlFunctions.h"


wxBoxSizer *CF::CreateFieldTextKey128(wxWindow *parent, int id, TextCtrlKeyHex128 **text_ctrl1, TextCtrlKeyHex128 **text_ctrl2, const Key128 &key, const wxString &label)
{
    wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);

    box->Add(new wxStaticText(parent, wxID_ANY, label));

    box->AddSpacer(5);

    *text_ctrl1 = new TextCtrlKeyHex128(parent, id, key, wxDefaultPosition);

    box->Add(*text_ctrl1);

    box->AddSpacer(5);

    *text_ctrl2 = new TextCtrlKeyHex128(parent, id + 1000, key, wxDefaultPosition);

    box->Add(*text_ctrl2);

    return box;
}


wxBoxSizer *CF::CreateButton(wxWindow *parent, wxButton **button, int id, const wxSize &size, const wxString &label)
{
    wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL);

    *button = new wxButton(parent, id, label, wxDefaultPosition, size);

    box->AddSpacer(20);

    box->Add(*button, 0, wxALIGN_CENTER);

    return box;
}
