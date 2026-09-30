// 2026/09/29 10:57:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/BJT.h"
#include "Utils/LineDrawer.h"


BJT::BJT(Test *test) : OStT(test)
{

}


void BJT::Draw(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    DrawCommon(dc, c);

    FuncAfterDraw(dc);
}


void BJT::DrawCommon(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    DrawCase(dc, c);

    DrawBase(dc, c);

    DrawCollectorEmitter(dc, c);

    FuncAfterDraw(dc);
}


void BJT::DrawBase(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    const wxPoint d = DeltaBase();

    wxPoint p1{ c.x - d.x, c.y - d.y };
    wxPoint p2{ c.x - d.x, c.y + d.y };

    dc.DrawLine(p1, p2);

    p1 = { c.x - d.x, c.y };
    p2 = { c.x - RADIUS - DR, c.y };

    point_B = p2;

    dc.DrawLine(p1, p2);
}


void BJT::DrawCollectorEmitter(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    const int dyb = (int)(RADIUS * 0.3);    // Смещение по базе
    const int dyc = (int)(RADIUS * 1.2);    // Смещение по коллектору

    const wxPoint d = DeltaBase();

    wxPoint p1{ c.x - d.x, c.y - dyb };
    wxPoint p2{ c.x + RADIUS, c.y - dyc };

    std::vector<wxPoint> points;

    {
        // Коллектор

        points = OStT::IntersectLineCircle(p1, p2, c, RADIUS);

        LineDriwer driwer{ dc, p1 };
        driwer.LineTo(points[1]);
        driwer.LineOnDY(-DR);
        point_C = driwer.GetCoord();
    }

    {
        // Эмиттер

        p1.y = c.y + dyb;
        p2.y = c.y + dyc;

        points = OStT::IntersectLineCircle(p1, p2, c, RADIUS);

        LineDriwer driwer{ dc, p1 };
        driwer.LineTo(points[1]);
        driwer.LineOnDY(DR);
        point_E = driwer.GetCoord();

        if (type == TypeBJT::NPN)
        {
            DrawArrow(dc, p1, points[1]);
        }
        else if (type == TypeBJT::PNP)
        {
            DrawArrow(dc, points[1], p1);
        }
    }
}
