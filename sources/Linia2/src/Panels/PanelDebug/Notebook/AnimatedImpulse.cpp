// 2026/03/19 16:46:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/AnimatedImpulse.h"


AnimatedImpulse::AnimatedImpulse(wxWindow *parent, const wxPoint &position) :
    PainterAnimated(parent, position, { WIDTH, HEIGHT })
{

}


void AnimatedImpulse::FuncDraw()
{
    wxPaintDC _dc(this);

    if (GetParent()->IsEnabled())
    {
        _dc.SetPen(wxPen(GetParent()->GetBackgroundColour()));
        _dc.SetBrush(wxBrush(GetParent()->GetBackgroundColour()));
    }
    else
    {
        _dc.SetPen(*wxWHITE_PEN);
        _dc.SetBrush(*wxBLACK_BRUSH);
    }

    _dc.DrawRectangle(0, 0, GetSize().x - 1, GetSize().y - 1);
}
