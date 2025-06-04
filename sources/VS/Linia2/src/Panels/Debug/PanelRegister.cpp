// 2025/6/4 10:27:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Debug/PanelRegister.h"
#include "Panels/Debug/PainterRegister.h"


PanelRegister::PanelRegister(wxWindow *parent, const wxString &title, int _bit_depth, bool _reverse_bits) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, { 800, 250 }, wxTAB_TRAVERSAL | wxSIMPLE_BORDER),
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
        choices.Add(_L("Команда"));

        new wxComboBox(this, wxID_ANY, choices[0], { x + 50, y - 2 }, { 150, 20 }, choices, wxCB_READONLY);

        new wxStaticText(this, wxID_ANY, _L("Команда"), { x + 300, y });

        choices.clear();

        choices.Add("0000 No operation");
        choices.Add("0001 Write to Input Register n (dependent on LDAC)");
        choices.Add("0010 Update DAC Register n with contents of Input Register n");
        choices.Add("0011 Write to and update DAC Channel n");
        choices.Add("0100 Power down/power up DAC");
        choices.Add("0101 Hardware LDAC mask register");
        choices.Add("0110 Software reset (power-on reset)");
        choices.Add("0111 Internal reference setup register");

        new wxComboBox(this, wxID_ANY, choices[0], { x + 360, y - 2 }, { 170, 20 }, choices, wxCB_READONLY);
    }

    {
        const int x0 = 40;      // / Отсюда начинаются
        const int y0 = 60;      // / галочки битов

        PainterRegister *painter = new PainterRegister(this, this, { 10, 85 }, { 750, 150 } );

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


void PanelRegister::SetNamesBits(const wxArrayString &_names)
{
    names_bits = _names;
}


void PanelRegister::SetDescriptionBits(int index, const std::vector<StructDescription> &_desc)
{
    desc[index] = _desc;
}
