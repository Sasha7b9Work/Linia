// 2025/6/4 10:27:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel12_Debug/PanelRegister.h"
#include "Panels/Panel12_Debug/PainterRegister.h"
#include "Controls/TextControls.h"


PanelRegister::PanelRegister(wxWindow *parent, const wxString &title, int _bit_depth, bool _additional_modes) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, { WIDTH, HEIGHT }, wxTAB_TRAVERSAL | wxSIMPLE_BORDER),
    bit_depth(_bit_depth),
    additional_modes(_additional_modes)
{
    SetName("PanelRegister");

    new wxStaticText(this, wxID_ANY, title, { 10, 10 });

    wxSize size_button{ 100, 30 };

    int x = 580;

    new wxButton(this, wxID_ANY, "Предустановки", { x, 0 }, size_button);

    new wxButton(this, wxID_ANY, "Записать", { x + 100, 0 }, size_button);

    int x0 = 10;
    int y0 = 40;

    if(additional_modes)
    {
        wxArrayString choices;
        choices.Add("Нижний уровень");
        choices.Add("Команда");

        new ButtonsCombo(this, "Режим", { x0, y0 - 2 }, 150, choices, choices, 1, "comboMode");

        choices.clear();

        choices.Add("0000 No operation");
        choices.Add("0001 Write to Input Register n (dependent on LDAC)");
        choices.Add("0010 Update DAC Register n with contents of Input Register n");
        choices.Add("0011 Write to and update DAC Channel n");
        choices.Add("0100 Power down/power up DAC");
        choices.Add("0101 Hardware LDAC mask register");
        choices.Add("0110 Software reset (power-on reset)");
        choices.Add("0111 Internal reference setup register");

        new ButtonsCombo(this, "Команда", { x0 + 170, y0 - 2 }, 400, choices, choices, 1, "comboCommand");

        y0 += 30;
    }

    {
        painter = new PainterRegister(this, this, { 10, y0  } );
    }

    Bind(wxEVT_RIGHT_DOWN, [](wxMouseEvent &event)
        {
            event.Skip(); // Пропустить событие дальше (родителю)
        });
}


void PanelRegister::SetNamesBits(const wxArrayString &_names)
{
    names_bits = _names;
}


void PanelRegister::SetDescriptionBits(int index, const std::vector<StructDescription> &_desc)
{
    desc[index] = _desc;

    if (index == 0)
    {
        for (auto &elem : desc[0])
        {
            if (elem.field.exist)
            {
                int num_bit = elem.first_bit + elem.num_bits - 1;

                int x = painter->BitX(num_bit, bit_depth) - 4;

                elem.field.text_ctrl = new TextCtrlNumber(painter, wxID_ANY, "", { x, (PainterRegister::W_B + 1) * 3 }, { PainterRegister::W_B * elem.num_bits, 20 }, 0, (1 << elem.num_bits) - 1);
            }
        }
    }
}
