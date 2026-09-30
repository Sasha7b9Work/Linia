// 2026/09/29 10:57:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/BJT.h"


BJT::BJT(Test *test) : OStT(test)
{

}


void BJT::Draw(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    DrawCommon(dc, c, true);

    FuncAfterDraw(dc);
}


void BJT::DrawCommon(AutoBufferedPaintDC &dc, const wxPoint &c, bool draw_case)
{
    if (draw_case)
    {
        DrawCase(dc, c);
    }

    DrawBase(dc, c);

    DrawCollectorEmitter(dc, c);
}


void BJT::DrawBase(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    const wxPoint d = DeltaBase();

    wxPoint p1{ c.x - d.x, c.y - d.y };
    wxPoint p2{ c.x - d.x, c.y + d.y };

    dc.DrawLine(p1, p2);

    p1 = { c.x - d.x, c.y };
    p2 = { c.x - radius - dr, c.y };

    point_B = p2;

    dc.DrawLine(p1, p2);
}


void BJT::DrawCollectorEmitter(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    const int dyb = (int)(radius * 0.3);    // Смещение по базе
    const int dyc = (int)(radius * 1.2);    // Смещение по коллектору

    const wxPoint d = DeltaBase();

    wxPoint p1{ c.x - d.x, c.y - dyb };
    wxPoint p2{ c.x + radius, c.y - dyc };

    std::vector<wxPoint> points;

    {
        // Коллектор

        points = OStT::IntersectLineCircle(p1, p2, c, radius);

        dc.MoveTo(p1);
        dc.LineTo(points[1]);
        dc.LineOnDY(-dr);
        point_C = dc.GetCoord();
    }

    {
        // Эмиттер

        p1.y = c.y + dyb;
        p2.y = c.y + dyc;

        points = OStT::IntersectLineCircle(p1, p2, c, radius);

        dc.MoveTo(p1);
        dc.LineTo(points[1]);
        dc.LineOnDY(dr);
        point_E = dc.GetCoord();

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
