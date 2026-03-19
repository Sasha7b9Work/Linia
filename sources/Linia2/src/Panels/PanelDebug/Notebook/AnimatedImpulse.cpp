// 2026/03/19 16:46:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/AnimatedImpulse.h"
#include "Utils/Timer.h"


AnimatedImpulse::AnimatedImpulse(wxWindow *parent, const wxPoint &position) :
    PainterAnimated(parent, position, { WIDTH, HEIGHT })
{

}


void AnimatedImpulse::FuncDraw()
{
    wxPaintDC _dc(this);

    static TimeMeterMS meter;

    _dc.SetBrush(wxBrush(GetParent()->GetBackgroundColour()));
    _dc.SetPen(wxPen(GetParent()->GetBackgroundColour()));


    if (GetParent()->IsEnabled())
    {
        _dc.DrawRectangle(0, 0, GetSize().x - 1, GetSize().y - 1);
    }
    else
    {
        _dc.DrawRectangle(0, 0, GetSize().x - 1, GetSize().y - 1);

        _dc.SetPen(*wxBLACK_PEN);

        static int x = 0;
        static int width = 100;

        if (meter.ElapsedMS() > 1)
        {
            x += 3;
            if (x > GetSize().x)
            {
                x = 0;
            }
        }

        int y = 10;

        _dc.DrawLine(0, 50, x, 50);
        _dc.DrawLine(x, 50, x, y);
        _dc.DrawLine(x, y, x + 10, y);
        _dc.DrawLine(x + 10, y, x + 10, 50);
        _dc.DrawLine(x + 10, 50, GetSize().x, 50);
    }
}
