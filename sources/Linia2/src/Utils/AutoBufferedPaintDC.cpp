// 2026/09/30 15:11:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Utils/AutoBufferedPaintDC.h"


void AutoBufferedPaintDC::MoveTo(const wxPoint &point)
{
    coord = point;
}


void AutoBufferedPaintDC::MoveToY(int y)
{
    coord.y = y;
}


void AutoBufferedPaintDC::MoveOnDY(int dy)
{
    coord.y += dy;
}


void AutoBufferedPaintDC::LineTo(const wxPoint &point)
{
    DrawLine(coord, point);

    coord = point;
}


void AutoBufferedPaintDC::LineToX(int x)
{
    DrawLine(coord, { x, coord.y });

    coord.x = x;
}


void AutoBufferedPaintDC::LineOnDY(int dy)
{
    DrawLine(coord, { coord.x, coord.y + dy });

    coord.y += dy;
}


void AutoBufferedPaintDC::LineOnDX(int dx)
{
    DrawLine(coord, { coord.x + dx, coord.y });

    coord.x += dx;
}


void AutoBufferedPaintDC::LineOn(const wxPoint &d)
{
    DrawLine(coord, { coord.x + d.x, coord.y + d.y });

    coord += d;
}


wxPoint AutoBufferedPaintDC::GetCoord() const
{
    return coord;
}
