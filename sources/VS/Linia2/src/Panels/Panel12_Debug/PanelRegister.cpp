// 2025/6/4 10:27:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel12_Debug/PanelRegister.h"
#include "Panels/Panel12_Debug/PainterRegister.h"


PanelRegister::PanelRegister(wxWindow *parent, const wxString &title, int _bit_depth, bool _reverse_bits, bool _additional_modes) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, { WIDTH, HEIGHT }, wxTAB_TRAVERSAL | wxSIMPLE_BORDER),
    bit_depth(_bit_depth),
    reverse_bits(_reverse_bits),
    additional_modes(_additional_modes)
{
    new wxStaticText(this, wxID_ANY, title, { 10, 10 });

    wxSize size_button{ 100, 30 };

    int x = 580;

    new wxButton(this, wxID_ANY, _L("Предустановки"), { x, 0 }, size_button);

    new wxButton(this, wxID_ANY, _L("Записать"), { x + 100, 0 }, size_button);

    int x0 = 10;
    int y0 = 40;

    if(additional_modes)
    {
        new wxStaticText(this, wxID_ANY, _L("Режим"), { x0, y0 });

        wxArrayString choices;
        choices.Add(_L("Нижний уровень"));
        choices.Add(_L("Команда"));

        new wxComboBox(this, wxID_ANY, choices[0], { x0 + 50, y0 - 2 }, { 150, 20 }, choices, wxCB_READONLY);

        new wxStaticText(this, wxID_ANY, _L("Команда"), { x0 + 300, y0 });

        choices.clear();

        choices.Add("0000 No operation");
        choices.Add("0001 Write to Input Register n (dependent on LDAC)");
        choices.Add("0010 Update DAC Register n with contents of Input Register n");
        choices.Add("0011 Write to and update DAC Channel n");
        choices.Add("0100 Power down/power up DAC");
        choices.Add("0101 Hardware LDAC mask register");
        choices.Add("0110 Software reset (power-on reset)");
        choices.Add("0111 Internal reference setup register");

        new wxComboBox(this, wxID_ANY, choices[0], { x0 + 360, y0 - 2 }, { 170, 20 }, choices, wxCB_READONLY);

        y0 += 30;
    }

    {
        new PainterRegister(this, this, { 10, y0  } );
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
