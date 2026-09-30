// 2026/09/29 11:15:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/FET.h"


FET::FET(Test *test) : OStT(test)
{

}


void FET::Draw(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    DrawCommon(dc, c);

    FuncAfterDraw(dc);
}


void FET::DrawCommon(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    DrawCase(dc, c);

    DrawGate(dc, c);

    DrawSourceDrain(dc, c);
}


void FET::DrawGate(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    wxPoint p1{ c.x - GateDX(), c.y - GateDY() };
    wxPoint p2{ c.x - GateDX(), c.y + GateDY() };

    dc.DrawLine(p1, p2);

    p1 = wxPoint{ c.x - GateDX(), c.y + DrainDY() };
    p2 = wxPoint{ c.x - radius - dr, c.y + DrainDY() };

    dc.DrawLine(p1, p2);

    point_B = p2;

    if (type == TypeFET::ChannelN)
    {
        DrawArrow(dc, p2, p1);
    }
    else if (type == TypeFET::ChannelP)
    {
        std::vector<wxPoint> points = IntersectLineCircle(p2, p1, c, radius);

        DrawArrow(dc, p1, points[0]);
    }
}


void FET::DrawSourceDrain(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    int dx = (int)std::sqrt(radius * radius - DrainDY() * DrainDY());

    wxPoint p1{ c.x, c.y - DrainDY() };
    wxPoint p2{ c.x + dx, p1.y };

    {
        // Сток

        dc.MoveTo(p1);
        dc.LineTo(p2);
        dc.LineOnDY(-dr);

        point_C = dc.GetCoord();
    }

    {
        // Исток

        p1.y = c.y + DrainDY();
        p2.y = p1.y;
        dc.MoveTo(p1);
        dc.LineTo(p2);
        dc.LineOnDY(dr);

        point_E = dc.GetCoord();
    }
}


int FET::GateDX()
{
    return 0;
}


int FET::GateDY()
{
    return (int)(radius * 0.8);
}


int FET::DrainDY()
{
    return (int)(radius * 0.55);
}
