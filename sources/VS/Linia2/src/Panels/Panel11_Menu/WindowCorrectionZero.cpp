// 2025/7/27 13:41:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/WindowCorrectionZero.h"


WindowCorretionZero::WindowCorretionZero() :
    Dialog(nullptr, wxID_ANY, _L("Коррекция смещения нуля"), wxDefaultPosition, { WIDTH, HEIGHT })
{
    int d = 10;

    int w = WIDTH / 2 - 3 * d;

    wxStaticBox *box = new wxStaticBox(this, wxID_ANY, _L("Канал") + " C", { d, d }, { w, 100 });
}
