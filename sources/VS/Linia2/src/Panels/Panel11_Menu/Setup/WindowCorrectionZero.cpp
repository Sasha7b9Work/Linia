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
    y = CreateRadioButton(this, d, y, &rbChanC_MeasI, _L("Измеритель") + " I");
    y = CreateRadioButton(this, d, y, &rbChanC_MeasU, _L("Измеритель") + " U");

    y = CreateLabelGroup(this, d, y, _L("Канал") + " B");
    y = CreateRadioButton(this, d, y, &rbChanB_MeasI, _L("Измеритель") + " I");
    y = CreateRadioButton(this, d, y, &rbChanB_MeasU, _L("Измеритель") + " U");
    y = CreateRadioButton(this, d, y, &rbChanB_SourceI, _L("Источник") + " I");
    y = CreateRadioButton(this, d, y, &rbChanB_SourceU, _L("Источник") + " U");

    y = CreateLabelGroup(this, d, y, _L("Канал") + " S");
    y = CreateRadioButton(this, d, y, &rbChanS_MeasI, _L("Измеритель") + " I");
    y = CreateRadioButton(this, d, y, &rbChanS_MeasU, _L("Измеритель") + " U");
    y = CreateRadioButton(this, d, y, &rbChanS_SourceI, _L("Источник") + " I");
    y = CreateRadioButton(this, d, y, &rbChanS_SourceU, _L("Источник") + " U");

    StaticBox *box = new StaticBox(this, _L("Внимание") + " !", { d + w + d, d }, { w, 200 });

    new wxStaticText(box, wxID_ANY,
        "Подключите контактирующее устройство и соедините гнёзда с помощью перемычек согласно схеме",
        { d, 50 }, { w - 20, 150 });

    wxSize size{ 70, BUTTON_HEIGHT };

    new wxButton(this, wxID_ANY, _L("Пуск"), { 50, y + 30 }, size);
    new wxButton(this, wxID_ANY, _L("Отмена"), { 200, y + 30 }, size);

    Bind(wxEVT_BUTTON, &WindowCorretionZero::OnEventButton, this);
}


void WindowCorretionZero::OnEventButton(wxCommandEvent &)
{

}
