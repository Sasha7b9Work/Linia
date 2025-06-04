// 2025/6/4 10:27:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Debug/PanelRegister.h"


PanelRegister::PanelRegister(wxWindow *parent, const wxString &title, int _bit_depth) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, {300, 300}, wxTAB_TRAVERSAL | wxSIMPLE_BORDER),
    bit_depth(_bit_depth)
{
    new wxStaticText(this, wxID_ANY, title, { 10, 10 });
}
