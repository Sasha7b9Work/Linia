// 2025/7/25 10:02:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/WindowScale.h"
#include "Utils/SystemDepend.h"


WindowScale::WindowScale() :
    Dialog(nullptr, wxID_ANY, _L("Шкала"), wxDefaultPosition, { WIDTH, HEIGHT })
{
    int width = 200;
    int height = 200;

    int d = 10;

    CreateBox(d, d, width, height, "X", "Ud, V", ID_SCALE_LINE_X_MIN, ID_SCALE_LINE_X_MAX);

    CreateBox(width + d * 2, d, width, height, "Y", "Id, mA", ID_SCALE_LINE_Y_MIN, ID_SCALE_LINE_Y_MAX);

    int y = height + d * 3;

    new wxStaticText(this, wxID_ANY, _L("Точность"), { d, y });

    new wxSpinCtrl(this, wxID_ANY, "", { 100, y }, { 70, TEXTCNTRL_HEIGHT});

    y += 50;

    new wxStaticText(this, wxID_ANY, _L("Число точек 1-ой очереди"), { d, y });

    wxArrayString choices;
    choices.Add("20");
    choices.Add("50");

    new wxComboBox(this, ID_SCALE_COMBO, choices[0], { 200, y }, { 70, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);
}


wxStaticBox *WindowScale::CreateBox(int x, int y, int w, int h, pchar axe, pchar units, int id_min, int id_max)
{
    wxStaticBox *box = new wxStaticBox(this, wxID_ANY, _L("Шкала по оси") + " " + axe, {x, y}, {w, h});

    y = 25;

    int d = 6;

    int dy = 35;

    new wxStaticText(box, wxID_ANY, units, { d / 2, SD::Y_SB(y) }, { w - d, 20 }, wxALIGN_CENTER);

    y += dy;

    new wxStaticText(box, wxID_ANY, "X min", { d, SD::Y_SB(y) });

    int s = 50;

    wxSize size_text{ 50, TEXTCNTRL_HEIGHT };

    new wxTextCtrl(box, id_min, "", { d + s, SD::Y_SB(y) }, size_text);

    y += dy;

    new wxStaticText(box, wxID_ANY, "X max", { d, SD::Y_SB(y) });

    new wxTextCtrl(box, id_max, "", { d + s, SD::Y_SB(y) }, size_text);

    y += dy * 3 / 2;

    new wxStaticText(box, wxID_ANY, _L("Шаг"), { 10, SD::Y_SB(y) });

    return box;
}
