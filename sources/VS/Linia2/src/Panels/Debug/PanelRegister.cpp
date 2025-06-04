// 2025/6/4 10:27:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Debug/PanelRegister.h"
#include "Panels/Debug/PainterRegister.h"


PanelRegister::PanelRegister(wxWindow *parent, const wxString &title, int _bit_depth, bool _reverse_bits) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, { 800, 200 }, wxTAB_TRAVERSAL | wxSIMPLE_BORDER),
    bit_depth(_bit_depth),
    reverse_bits(_reverse_bits)
{
    new wxStaticText(this, wxID_ANY, title, { 10, 10 });

    {
        int x = 10;
        int y = 50;

        new wxStaticText(this, wxID_ANY, _L("Режим"), { x, y });

        wxArrayString choices;
        choices.Add(_L("Нижний уровень"));

        new wxComboBox(this, wxID_ANY, choices[0], { x + 50, y - 2 }, { 150, 20 }, choices, wxCB_READONLY);
    }

    {
        const int x0 = 40;
        const int y0 = 10;

        PainterRegister *painter = new PainterRegister(this, { 10, 85 }, { 800, 100 } );

        wxSize size(20, 20);

        for (int i = 0; i < bit_depth; i++)
        {
            new wxCheckBox(painter, wxID_ANY, "", { x0 + i * size.x, y0 }, size);
        }

        if (reverse_bits)
        {
            new wxStaticText(painter, wxID_ANY, "DB0", { x0 - 35, y0 + 2 });
            new wxStaticText(painter, wxID_ANY, wxString::Format("DB%d", bit_depth - 1), { x0 + size.x * bit_depth + 3, y0 + 2 });
        }
        else
        {
            new wxStaticText(painter, wxID_ANY, wxString::Format("DB%d", bit_depth - 1), { x0 - 35, y0 + 2 });
            new wxStaticText(painter, wxID_ANY, "DB0", { x0 + size.x * bit_depth + 3, y0 + 2 });
        }
    }
}
