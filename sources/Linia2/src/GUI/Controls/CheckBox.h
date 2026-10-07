// 2026/04/29 16:45:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#pragma warning(push, 0)
#include <wx/checkbox.h>
#pragma warning(pop)


class CheckBox : public wxCheckBox
{
public:

    CheckBox(wxWindow *, const wxString &);
};
