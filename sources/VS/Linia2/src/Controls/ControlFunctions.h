// 2025/03/20 08:10:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/TextControls.h"


namespace CF
{
    wxBoxSizer *CreateFieldTextKey128(wxWindow *parent, int id, TextCtrlKeyHex128 **, TextCtrlKeyHex128 **, const Key128 &, const wxString &);

    wxBoxSizer *CreateButton(wxWindow *parent, wxButton **, int id, const wxSize &, const wxString &);
}
