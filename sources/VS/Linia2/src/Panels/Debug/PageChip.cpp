// 2025/6/3 14:01:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Debug/PageChip.h"


PageChip::PageChip(wxNotebook *parent, const wxString &title) :
    wxPanel(parent, wxID_ANY)
{
    wxPanel::SetName(title);

    wxPanel::SetBackgroundColour(parent->GetThemeBackgroundColour());
}
