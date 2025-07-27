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

    int w = WIDTH / 2 - 3 * d;

    int h = 80;

    wxStaticBox *box = new wxStaticBox(this, wxID_ANY, _L("Канал") + " C", { d, d }, { w, h });

    {
        new wxRadioButton(box, ID_RB_CHAN_C_MEAS_I, _L("Измеритель") + " I", { d, SD::Y_SB(x_rb) }, wxDefaultSize, wxRB_GROUP);
        new wxRadioButton(box, ID_RB_CHAN_C_MEAS_U, _L("Измеритель") + " U", { d ,SD::Y_SB(x_rb + drb) });
    }

    int h2 = 130;

    box = new wxStaticBox(this, wxID_ANY, _L("Канал") + " B", { d, d * 2 + h }, { w, h2 });

    {
        new wxRadioButton(box, ID_RB_CHAN_B_MEAS_I, _L("Измеритель") + " I", { d, SD::Y_SB(x_rb) });
        new wxRadioButton(box, ID_RB_CHAN_B_MEAS_U, _L("Измеритель") + " U", { d, SD::Y_SB(x_rb + drb) });
        new wxRadioButton(box, ID_RB_CHAN_B_SOURCE_I, _L("Источник") + " I", { d, SD::Y_SB(x_rb + 2 * drb) });
        new wxRadioButton(box, ID_RB_CHAN_B_SOURCE_U, _L("Источник") + " U", { d, SD::Y_SB(x_rb + 3 * drb) });
    }

    h += d + h2;

    box = new wxStaticBox(this, wxID_ANY, _L("Канал") + " S", { d, d * 2 + h }, { w, h2 });

    {
        new wxRadioButton(box, ID_RB_CHAN_S_MEAS_I, _L("Измеритель") + " I", { d, SD::Y_SB(x_rb) });
        new wxRadioButton(box, ID_RB_CHAN_S_MEAS_U, _L("Измеритель") + " U", { d, SD::Y_SB(x_rb + drb) });
        new wxRadioButton(box, ID_RB_CHAN_S_SOURCE_I, _L("Источник") + " I", { d, SD::Y_SB(x_rb + 2 * drb) });
        new wxRadioButton(box, ID_RB_CHAN_S_SOURCE_U, _L("Источник") + " U", { d, SD::Y_SB(x_rb + 3 * drb) });
    }

    Bind(wxEVT_RADIOBUTTON, &WindowCorretionZero::OnEventRadioButton, this);
}


void WindowCorretionZero::OnEventRadioButton(wxCommandEvent &)
{

}
