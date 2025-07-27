// 2025/7/27 19:54:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/Setup/WindowCalibrate.h"


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

    wxStaticBox *box = new wxStaticBox(this, wxID_ANY, "Параметры режима калибровки", { d, y + 30 }, { 300, 200 });


}
