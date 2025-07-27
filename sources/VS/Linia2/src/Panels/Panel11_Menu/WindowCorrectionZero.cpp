// 2025/7/27 13:41:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/WindowCorrectionZero.h"
#include "Utils/SystemDepend.h"


WindowCorretionZero::WindowCorretionZero() :
    Dialog(nullptr, wxID_ANY, _L("Коррекция смещения нуля"), wxDefaultPosition, { WIDTH, HEIGHT })
{
    int drb = 20;
    int x_rb = 20;

    int d = 10;

    const int w = WIDTH / 2 - 3 * d;

    int h = 80;

    wxStaticBox *box = new wxStaticBox(this, wxID_ANY, _L("Канал") + " C", { d, d }, { w, h });

    {
        buttons.push_back(new wxRadioButton(box, ID_RB_CHAN_C_MEAS_I, _L("Измеритель") + " I", { d, SD::Y_SB(x_rb) }, wxDefaultSize, wxRB_GROUP));
        buttons.push_back(new wxRadioButton(box, ID_RB_CHAN_C_MEAS_U, _L("Измеритель") + " U", { d ,SD::Y_SB(x_rb + drb) }));
    }

    int h2 = 130;

    box = new wxStaticBox(this, wxID_ANY, _L("Канал") + " B", { d, d * 2 + h }, { w, h2 });

    {
        buttons.push_back(new wxRadioButton(box, ID_RB_CHAN_B_MEAS_I, _L("Измеритель") + " I", { d, SD::Y_SB(x_rb) }));
        buttons.push_back(new wxRadioButton(box, ID_RB_CHAN_B_MEAS_U, _L("Измеритель") + " U", { d, SD::Y_SB(x_rb + drb) }));
        buttons.push_back(new wxRadioButton(box, ID_RB_CHAN_B_SOURCE_I, _L("Источник") + " I", { d, SD::Y_SB(x_rb + 2 * drb) }));
        buttons.push_back(new wxRadioButton(box, ID_RB_CHAN_B_SOURCE_U, _L("Источник") + " U", { d, SD::Y_SB(x_rb + 3 * drb) }));
    }

    h += d + h2;

    box = new wxStaticBox(this, wxID_ANY, _L("Канал") + " S", { d, d * 2 + h }, { w, h2 });

    {
        buttons.push_back(new wxRadioButton(box, ID_RB_CHAN_S_MEAS_I, _L("Измеритель") + " I", { d, SD::Y_SB(x_rb) }));
        buttons.push_back(new wxRadioButton(box, ID_RB_CHAN_S_MEAS_U, _L("Измеритель") + " U", { d, SD::Y_SB(x_rb + drb) }));
        buttons.push_back(new wxRadioButton(box, ID_RB_CHAN_S_SOURCE_I, _L("Источник") + " I", { d, SD::Y_SB(x_rb + 2 * drb) }));
        buttons.push_back(new wxRadioButton(box, ID_RB_CHAN_S_SOURCE_U, _L("Источник") + " U", { d, SD::Y_SB(x_rb + 3 * drb) }));
    }

    box = new wxStaticBox(this, wxID_ANY, _L("Внимание") + " !", {d + w + d, d}, {w, 200});

    new wxStaticText(box, wxID_ANY,
        "Подключите контактирующее устройство и соедините гнёзда с помощью перемычек согласно схеме",
        { d, 50 }, { w - 20, 150 });

    Bind(wxEVT_RADIOBUTTON, &WindowCorretionZero::OnEventRadioButton, this);
}


void WindowCorretionZero::OnEventRadioButton(wxCommandEvent &event)
{
    int id = event.GetId();

    for (auto btn : buttons)
    {
        if (btn->GetId() != id)
        {
            btn->SetValue(false);
        }
    }
}
