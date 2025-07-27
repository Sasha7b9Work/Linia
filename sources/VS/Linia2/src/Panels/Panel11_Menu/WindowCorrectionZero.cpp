// 2025/7/27 13:41:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/WindowCorrectionZero.h"


enum
{
    ID_RB_CHAN_C_MEAS_I = wxID_HIGHEST + 1,
    ID_RB_CHAN_C_MEAS_U,
    ID_RB_CHAN_B_MEAS_I,
    ID_RB_CHAN_B_MEAS_U,
    ID_RB_CHAN_B_SOURCE_I,
    ID_RB_CHAN_B_SOURCE_U,
    ID_RB_CHAN_S_MEAS_I,
    ID_RB_CHAN_S_MEAS_U,
    ID_RB_CHAN_S_SOURCE_I,
    ID_RB_CHAN_S_SOURCE_U,
    ID_BTN_CANCEL
};


WindowCorretionZero::WindowCorretionZero() :
    Dialog(nullptr, wxID_ANY, _L("Коррекция смещения нуля"), wxDefaultPosition, { WIDTH, HEIGHT })
{
    int d = 10;

    int w = WIDTH - 3 * d;

    wxStaticBox *box = new wxStaticBox(this, wxID_ANY, _L("Канал") + " C", { d, d }, { w, 100 });
}
