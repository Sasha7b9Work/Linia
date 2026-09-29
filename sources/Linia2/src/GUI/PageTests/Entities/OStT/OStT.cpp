// 2026/09/29 11:03:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/OStT.h"
#include "Utils/LineDrawer.h"


void OStT::DrawGround(int x, int y, wxAutoBufferedPaintDC &dc)
{
    dc.DrawLine(x - 10, y, x + 10, y);
}


void OStT::DrawLineWithAngle(const wxPoint &start, double length, double angleDeg, wxAutoBufferedPaintDC &dc)
{
    double angleRad = angleDeg * M_PI / 180.0;

    int endX = start.x + (int)(length * cos(angleRad));
    int endY = start.y - (int)(length * sin(angleRad));  // минус, т.к. Y вниз

    dc.DrawLine(start.x, start.y, endX, endY);
}


void OStT::DrawCase(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    dc.DrawCircle(c, RADIUS);
}


void OStT::DrawAnchorPoint(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    dc.DrawCircle(c, r);
}


void OStT::DrawArrow(wxAutoBufferedPaintDC &dc, const wxPoint &p1, const wxPoint &p2)
{
    wxPoint p = RotatePoint(p1, p2, 30.0);

    dc.DrawLine(p, p2);

    p = RotatePoint(p1, p2, -30.0);

    dc.DrawLine(p, p2);
}

wxPoint OStT::RotatePoint(const wxPoint &p1, const wxPoint &p2, double angleDeg)
{
    // Вектор от p2 к p1
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;

    // Угол в радианах
    double angleRad = angleDeg * M_PI / 180.0;

    double cosA = std::cos(angleRad);
    double sinA = std::sin(angleRad);

    // Повёрнутый вектор
    double newDx = dx * cosA - dy * sinA;
    double newDy = dx * sinA + dy * cosA;

    // Новая точка
    return wxPoint(
        p2.x + static_cast<int>(newDx + 0.5),
        p2.y + static_cast<int>(newDy + 0.5)
    );
}
