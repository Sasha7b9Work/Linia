// 2025/6/4 10:27:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Debug/PanelRegister.h"


PanelRegister::PanelRegister(wxWindow *parent, const wxString &title, int _bit_depth, bool _reverse_bits) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, { 800, 200 }, wxTAB_TRAVERSAL | wxSIMPLE_BORDER),
    bit_depth(_bit_depth),
    reverse_bits(_reverse_bits)
{
    new wxStaticText(this, wxID_ANY, title, { 10, 10 });

    const int x0 = 50;
    const int y0 = 50;

    wxSize size(20, 20);

    for (int i = 0; i < bit_depth; i++)
    {
        new wxCheckBox(this, wxID_ANY, "", { x0 + i * size.x, y0 }, size);
    }

    if (reverse_bits)
    {
        new wxStaticText(this, wxID_ANY, "DB0", { x0 - 35, y0 + 2 });
        new wxStaticText(this, wxID_ANY, wxString::Format("DB%d", bit_depth - 1), { x0 + size.x * bit_depth + 3, y0 + 2 });
    }
    else
    {
        new wxStaticText(this, wxID_ANY, wxString::Format("DB%d", bit_depth - 1), { x0 - 35, y0 + 2 });
        new wxStaticText(this, wxID_ANY, "DB0", { x0 + size.x * bit_depth + 3, y0 + 2 });
    }
}
