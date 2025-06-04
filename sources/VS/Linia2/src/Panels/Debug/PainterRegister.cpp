// 2025/6/4 15:50:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Debug/PainterRegister.h"
#include "Panels/Debug/PanelRegister.h"


PainterRegister::PainterRegister(wxWindow *parent, PanelRegister *_panel, const wxPoint &position, const wxSize &size) :
    Painter::Painter(parent, position, size),
    panel(_panel)
{

}


void PainterRegister::OnPaint(wxPaintEvent &)
{
    wxPaintDC _dc(this);

    wxGraphicsContext *gc = wxGraphicsContext::Create(_dc);

    gc->SetPen(*wxBLACK_PEN);
    gc->SetBrush(wxBrush(*wxWHITE));
    gc->DrawRectangle(0, 0, GetSize().x - 1, GetSize().y - 1);

    int num_bits = panel->bit_depth;

    int x0 = 36;
    int y0 = 40;
    int w = 20;
    int h = w;

    for (int i = 0; i < num_bits; i++)
    {
        gc->DrawRectangle(x0 + i * w, y0, w, h);
    }

    delete gc;
}