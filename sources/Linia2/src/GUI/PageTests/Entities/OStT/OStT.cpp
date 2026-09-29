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
