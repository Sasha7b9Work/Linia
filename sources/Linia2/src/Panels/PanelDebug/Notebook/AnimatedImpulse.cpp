// 2026/03/19 16:46:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/AnimatedImpulse.h"


AnimatedImpulse::AnimatedImpulse(wxWindow *parent, const wxPoint &position, const wxColor &background) :
    PainterAnimated(parent, position, { WIDTH, HEIGHT }),
    color_background(background)
{
    wxWindow::Enable(false);
}


void AnimatedImpulse::FuncDraw()
{
    wxPaintDC _dc(this);

    _dc.SetBrush(wxBrush(color_background));
    _dc.SetPen(wxPen(color_background));


    if(!IsEnabled())
    {
        _dc.DrawRectangle(0, 0, GetSize().x - 1, GetSize().y - 1);
    }
    else
    {
        if (meter.ElapsedMS() > 10)
        {
            meter.Reset();

            x += 5;
            if (x > GetSize().x)
            {
                x = 0;
            }
        }

        _dc.DrawRectangle(0, 0, GetSize().x - 1, GetSize().y - 1);

        _dc.SetPen(*wxBLACK_PEN);


        int y = 10;

        _dc.DrawLine(0, 50, x, 50);
        _dc.DrawLine(x, 50, x, y);
        _dc.DrawLine(x, y, x + 10, y);
        _dc.DrawLine(x + 10, y, x + 10, 50);
        _dc.DrawLine(x + 10, 50, GetSize().x, 50);
    }
}
