// 2025/7/27 13:41:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/Setup/WindowCorrectionZero.h"
#include "Utils/SystemDepend.h"
#include "Controls/StaticBox.h"


WindowCorretionZero::WindowCorretionZero() :
    Dialog("Коррекция смещения нуля", wxDefaultPosition, { WIDTH, HEIGHT })
{
    int d = 10;
    const int w = WIDTH / 2 - 3 * d;
    int y = d + TopY();

    wxWindow *wnd = MainWidget();

    y = CreateLabelGroup(wnd, d, y, "Канал C");
    y = CreateRadioButton(wnd, d, y, &rbChanC_MeasI, "Измеритель I");
    y = CreateRadioButton(wnd, d, y, &rbChanC_MeasU, "Измеритель U");

    y = CreateLabelGroup(wnd, d, y, "Канал B");
    y = CreateRadioButton(wnd, d, y, &rbChanB_MeasI, "Измеритель I");
    y = CreateRadioButton(wnd, d, y, &rbChanB_MeasU, "Измеритель U");
    y = CreateRadioButton(wnd, d, y, &rbChanB_SourceI, "Источник I");
    y = CreateRadioButton(wnd, d, y, &rbChanB_SourceU, "Источник U");

    y = CreateLabelGroup(wnd, d, y, "Канал S");
    y = CreateRadioButton(wnd, d, y, &rbChanS_MeasI, "Измеритель I");
    y = CreateRadioButton(wnd, d, y, &rbChanS_MeasU, "Измеритель U");
    y = CreateRadioButton(wnd, d, y, &rbChanS_SourceI, "Источник I");
    y = CreateRadioButton(wnd, d, y, &rbChanS_SourceU, "Источник U");

    StaticBox *box = new StaticBox(wnd, "Внимание !", { d + w + d, SD::Y_SB(TopY() + d) }, { w, 200 });

    new wxStaticText(box, wxID_ANY,
        "Подключите контактирующее устройство и соедините гнёзда с помощью перемычек согласно схеме",
        { d, SD::Y_SB(50) }, { w - 20, 150 });

    wxSize size{ 70, BUTTON_HEIGHT };

    new wxButton(wnd, wxID_ANY, "Пуск", { 50, y + 30 }, size);
    new wxButton(wnd, wxID_ANY, "Отмена", { 200, y + 30 }, size);

    Bind(wxEVT_BUTTON, &WindowCorretionZero::OnEventButton, this);
}


void WindowCorretionZero::OnEventButton(wxCommandEvent &)
{

}
