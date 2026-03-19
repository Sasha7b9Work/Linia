// 2026/03/19 16:46:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/AnimatedImpulse.h"


AnimatedImpulse::AnimatedImpulse(wxWindow *parent, const wxPoint &position) :
    PainterAnimated(parent, position, { WIDTH, HEIGHT })
{

}


void AnimatedImpulse::FuncDraw()
{
    wxPaintDC dc(this);

    if (GetParent()->IsEnabled())
    {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(*wxWHITE_BRUSH);
    }
    else
    {
        dc.SetPen(*wxWHITE_PEN);
        dc.SetBrush(*wxBLACK_BRUSH);
    }
    dc.DrawRectangle(0, 0, GetSize().x - 1, GetSize().y - 1);
}
