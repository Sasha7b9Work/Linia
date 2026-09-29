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


void OStT::DrawCircle(wxAutoBufferedPaintDC &dc, const wxPoint &c, int &x_col, int &y_col)
{
    int r = 5;

    x_col = c.x + RADIUS / 2;   // / Координаты точки коммутации
    y_col = c.y - 2 * RADIUS;   // / с коллектором

    LineDriwer driwer(dc, x_col, y_col);
    driwer.LineTo(c.x + RADIUS / 2, c.y + 2 * RADIUS);                  // Вертикальная линия, которая выходит из коллектора и эмиттера
    DrawGround(driwer.GetX(), driwer.GetY(), dc);
    driwer.MoveOnDY(-20);
    point_emitter = driwer.GetCoord();
    dc.DrawCircle(point_emitter, r);
}
