// 2025/7/27 19:54:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/Setup/WindowCalibrate.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/SystemDepend.h"


WindowCalibrate::WindowCalibrate() :
    Dialog(nullptr, wxID_ANY, _L("Калибровка"), wxDefaultPosition, {WIDTH, HEIGHT})
{
    int d = 20;

    int y = 20;

    new wxStaticText(this, wxID_ANY, "Серийный номер прибора", { d, y });
    textSerialNumber = new wxStaticText(this, wxID_ANY, "000", { 200, y });

    y = 50;

    new wxStaticText(this, wxID_ANY, "Дата последней калибровки", { d, y });
    textDateTime = new wxStaticText(this, wxID_ANY, "", { 200, y });

    textDateTime->SetLabel(wxDateTime::Now().Format("%d.%m.%Y"));

    new wxButton(this, ID_CALIB_BTN_SAVE, "Сохранить", { 300, 30 }, { 75, BUTTON_HEIGHT });

    wxStaticBox *box = new wxStaticBox(this, wxID_ANY, "Параметры режима калибровки", { d, y + 30 }, { 270, 330 });

    {
        y = d + 10;

        new wxStaticText(box, wxID_ANY, "Диапазон", { d, SD::Y_SB(y) } );

        wxArrayString choices;
        choices.push_back("50A");

        comboRange = new wxComboBox(box, wxID_ANY, choices[0], { d + 75, SD::Y_SB(y) }, { 75, TEXTCNTRL_HEIGHT }, choices, wxCB_SIMPLE | wxCB_DROPDOWN | wxCB_READONLY);

        y += d;

        y = CreateLabelGroup(box, d, y, "Канал C");
        y = CreateRadioButton(box, d, y,  &rbChanC_MeasI, "Измеритель I");
        rbChanC_MeasI->SetFocus();
        y = CreateRadioButton(box, d, y, &rbChanC_MeasU, "Измеритель U");

        y = CreateLabelGroup(box, d, y, "Канал B");
        y = CreateRadioButton(box, d, y, &rbChanB_MeasI, "Источник I / Измеритель I");
        y = CreateRadioButton(box, d, y, &rbChanB_MeasU, "Источник U / Измеритель U");

        y = CreateLabelGroup(box, d, y, "Канал S");
        y = CreateRadioButton(box, d, y, &rbChanS_MeasI, "Источник I / Измеритель I");
        y = CreateRadioButton(box, d, y, &rbChanS_MeasU, "Источник U / Измеритель U");
    }

    y = 430;

    box = new wxStaticBox(this, wxID_ANY, "Внимание!", { d, y }, { 300, 100 });

    new wxStaticText(box, wxID_ANY, "Соберите схему для определения основной погрешности измерения "
        "тока по каналу C на диапазонах от 5 A до 50 A (рис. В.8 РЭ) и установите резистор Rn",
        { d, SD::Y_SB(d) }, { 260, 75 });

    y += box->GetSize().y;

    wxSize size{ 70, BUTTON_HEIGHT };

    new wxButton(this, ID_CALIB_BTN_START, _L("Пуск"), { 50, y + 30 }, size);
    new wxButton(this, ID_CALIB_BTN_CANCEL, _L("Отмена"), { 200, y + 30 }, size);

    Bind(wxEVT_BUTTON, &WindowCalibrate::OnEventButton, this);
}


void WindowCalibrate::OnEventButton(wxCommandEvent &)
{

}
