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

    y = CreateLabelGroup(d, y, _L("Канал") + " C");
    y = CreateRadioButton(d, y, ID_RB_CHAN_C_MEAS_I, _L("Измеритель") + " I");
    y = CreateRadioButton(d, y, ID_RB_CHAN_C_MEAS_U, _L("Измеритель") + " U");

    y += 30;

    y = CreateLabelGroup(d, y, _L("Канал") + " B");
    y = CreateRadioButton(d, y, ID_RB_CHAN_B_MEAS_I, _L("Измеритель") + " I");
    y = CreateRadioButton(d, y, ID_RB_CHAN_B_MEAS_U, _L("Измеритель") + " U");
    y = CreateRadioButton(d, y, ID_RB_CHAN_B_SOURCE_I, _L("Источник") + " I");
    y = CreateRadioButton(d, y, ID_RB_CHAN_B_SOURCE_U, _L("Источник") + " U");

    y += 30;

    y = CreateLabelGroup(d, y, _L("Канал") + " S");
    y = CreateRadioButton(d, y, ID_RB_CHAN_S_MEAS_I, _L("Измеритель") + " I");
    y = CreateRadioButton(d, y, ID_RB_CHAN_S_MEAS_U, _L("Измеритель") + " U");
    y = CreateRadioButton(d, y, ID_RB_CHAN_S_SOURCE_I, _L("Источник") + " I");
    y = CreateRadioButton(d, y, ID_RB_CHAN_S_SOURCE_U, _L("Источник") + " U");

    wxStaticBox *box = new wxStaticBox(this, wxID_ANY, _L("Внимание") + " !", { d + w + d, d }, { w, 200 });

    new wxStaticText(box, wxID_ANY,
        "Подключите контактирующее устройство и соедините гнёзда с помощью перемычек согласно схеме",
        { d, 50 }, { w - 20, 150 });

    wxSize size{ 70, BUTTON_HEIGHT };

    new wxButton(this, ID_ZERO_BTN_START, _L("Пуск"), { 50, y + 30 }, size);
    new wxButton(this, ID_ZERO_BTN_CANCEL, _L("Отмена"), { 200, y + 30 }, size);

    Bind(wxEVT_RADIOBUTTON, &WindowCorretionZero::OnEventRadioButton, this);
    Bind(wxEVT_BUTTON, &WindowCorretionZero::OnEventButton, this);
}


int WindowCorretionZero::CreateLabelGroup(int x, int y, const wxString &label)
{
    new wxStaticLine(this, wxID_ANY, { x, SD::Y_SB(y) }, { 100, -1 }, wxLI_HORIZONTAL);

    y += 7;

    new wxStaticText(this, wxID_ANY, label, { 20, SD::Y_SB(y) });

    return y + 25;
}


int WindowCorretionZero::CreateRadioButton(int x, int y, int id, const wxString &label)
{
    buttons.push_back(new wxRadioButton(this, id, label, { x, SD::Y_SB(y) }));

    return y + 20;
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


void WindowCorretionZero::OnEventButton(wxCommandEvent &)
{

}
