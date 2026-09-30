// 2026/09/29 11:15:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/FET.h"
#include "Utils/LineDrawer.h"


FET::FET(Test *test) : OStT(test)
{

}


void FET::Draw(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    DrawCommon(dc, c);

    FuncAfterDraw(dc);
}


void FET::DrawCommon(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    DrawCase(dc, c);

    DrawGate(dc, c);

    DrawSourceDrain(dc, c);
}


void FET::DrawGate(wxAutoBufferedPaintDC &dc, const wxPoint &c)
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


void FET::DrawSourceDrain(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    int dx = (int)std::sqrt(radius * radius - DrainDY() * DrainDY());

    wxPoint p1{ c.x, c.y - DrainDY() };
    wxPoint p2{ c.x + dx, p1.y };

    {
        // Сток

        LineDriwer driwer(dc, p1);
        driwer.LineTo(p2);
        driwer.LineOnDY(-dr);

        point_C = driwer.GetCoord();
    }

    {
        // Исток

        p1.y = c.y + DrainDY();
        p2.y = p1.y;
        LineDriwer driwer(dc, p1);
        driwer.LineTo(p2);
        driwer.LineOnDY(dr);

        point_E = driwer.GetCoord();
    }
}


int FET::GateDX()
{
    return 0;
}


int FET::GateDY()
{
    return (int)(radius * 0.8f);
}


int FET::DrainDY()
{
    return (int)(radius * 0.55f);
}
