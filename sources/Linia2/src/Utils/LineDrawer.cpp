// 2026/09/04 10:14:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Utils/LineDrawer.h"


LineDriwer::LineDriwer(wxAutoBufferedPaintDC &_dc, const wxPoint &_point) : dc(_dc), coord{ _point }
{
}


int LineDriwer::LineOnDY(int dy)
{
    wxPoint coord_next{ coord.x, coord.y + dy };

    dc.DrawLine(coord, coord_next);

    coord = coord_next;

    return coord.y;
}


void LineDriwer::MoveTo(int x, int y)
{
    coord = { x, y };
}


void LineDriwer::MoveTo(const wxPoint &new_coord)
{
    coord = new_coord;
}


int LineDriwer::LineToY(int y)
{
    wxPoint coord_next{ coord.x, y };

    dc.DrawLine(coord, coord_next);

    coord = coord_next;

    return coord.y;
}


int LineDriwer::LineToX(int x)
{
    wxPoint coord_next{ x, coord.y };

    dc.DrawLine(coord, coord_next);

    coord = coord_next;

    return coord.y;
}


wxPoint LineDriwer::LineTo(const wxPoint &_point)
{
    dc.DrawLine(coord, _point);

    coord = _point;

    return coord;
}


int LineDriwer::LineOnDX(int dx)
{
    wxPoint coord_next{ coord.x + dx, coord.y };

    dc.DrawLine(coord, coord_next);

    coord = coord_next;

    return coord.x;
}


wxPoint LineDriwer::LineOn(const wxPoint &delta)
{
    wxPoint coord_next{ coord.x + delta.x, coord.y + delta.y };
    dc.DrawLine(coord, coord_next);
    coord = coord_next;
    return coord;
}
