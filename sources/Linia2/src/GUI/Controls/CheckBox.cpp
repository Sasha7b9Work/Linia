// 2026/04/29 16:46:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/Controls/CheckBox.h"


CheckBox::CheckBox(wxWindow *parent, const wxString &text) :
    wxCheckBox(parent, wxID_ANY, text)
{
    wxCheckBox::SetCursor(wxCursor(wxCURSOR_HAND));
}
