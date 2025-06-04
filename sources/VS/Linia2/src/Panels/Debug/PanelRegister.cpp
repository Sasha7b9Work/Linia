// 2025/6/4 10:27:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Debug/PanelRegister.h"


PanelRegister::PanelRegister(wxWindow *parent, const wxString &title, int _bit_depth) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, { 500, 200 }, wxTAB_TRAVERSAL | wxSIMPLE_BORDER),
    bit_depth(_bit_depth)
{
    new wxStaticText(this, wxID_ANY, title, { 10, 10 });

    int x = 10;
    int y = 35;

    wxSize size(13, 13);

    for (int i = 0; i < bit_depth; i++)
    {
        new wxCheckBox(this, wxID_ANY, "", { x + i * size.x, y }, size);
    }
}
