// 2025/7/27 13:41:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/Setup/WindowCorrectionZero.h"
#include "Utils/SystemDepend.h"


WindowCorretionZero::WindowCorretionZero() :
    Dialog(nullptr, wxID_ANY, _L("Коррекция смещения нуля"), wxDefaultPosition, { WIDTH, HEIGHT })
{
    int d = 10;
    const int w = WIDTH / 2 - 3 * d;
    int y = d;

    y = CreateLabelGroup(this, d, y, _L("Канал") + " C");
    y = CreateRadioButton(this, d, y, ID_RB_CHAN_C_MEAS_I, _L("Измеритель") + " I");
    y = CreateRadioButton(this, d, y, ID_RB_CHAN_C_MEAS_U, _L("Измеритель") + " U");

    y = CreateLabelGroup(this, d, y, _L("Канал") + " B");
    y = CreateRadioButton(this, d, y, ID_RB_CHAN_B_MEAS_I, _L("Измеритель") + " I");
    y = CreateRadioButton(this, d, y, ID_RB_CHAN_B_MEAS_U, _L("Измеритель") + " U");
    y = CreateRadioButton(this, d, y, ID_RB_CHAN_B_SOURCE_I, _L("Источник") + " I");
    y = CreateRadioButton(this, d, y, ID_RB_CHAN_B_SOURCE_U, _L("Источник") + " U");

    y = CreateLabelGroup(this, d, y, _L("Канал") + " S");
    y = CreateRadioButton(this, d, y, ID_RB_CHAN_S_MEAS_I, _L("Измеритель") + " I");
    y = CreateRadioButton(this, d, y, ID_RB_CHAN_S_MEAS_U, _L("Измеритель") + " U");
    y = CreateRadioButton(this, d, y, ID_RB_CHAN_S_SOURCE_I, _L("Источник") + " I");
    y = CreateRadioButton(this, d, y, ID_RB_CHAN_S_SOURCE_U, _L("Источник") + " U");

    wxStaticBox *box = new wxStaticBox(this, wxID_ANY, _L("Внимание") + " !", { d + w + d, d }, { w, 200 });

    new wxStaticText(box, wxID_ANY,
        "Подключите контактирующее устройство и соедините гнёзда с помощью перемычек согласно схеме",
        { d, 50 }, { w - 20, 150 });

    wxSize size{ 70, BUTTON_HEIGHT };

    new wxButton(this, ID_ZERO_BTN_START, _L("Пуск"), { 50, y + 30 }, size);
    new wxButton(this, ID_ZERO_BTN_CANCEL, _L("Отмена"), { 200, y + 30 }, size);

    Bind(wxEVT_BUTTON, &WindowCorretionZero::OnEventButton, this);
}


void WindowCorretionZero::OnEventButton(wxCommandEvent &)
{

}
