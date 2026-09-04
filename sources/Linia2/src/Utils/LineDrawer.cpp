// 2026/09/04 10:14:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Utils/LineDrawer.h"


LineDriwer::LineDriwer(wxPaintDC &_dc, int _x, int _y) : dc(_dc), coord{ _x, _y }
{
}


int LineDriwer::AppendY(int dy)
{
    wxPoint coord_next{ coord.x, coord.y + dy };

    dc.DrawLine(coord, coord_next);

    coord = coord_next;

    return coord.y;
}


int LineDriwer::AppendX(int dx)
{
    wxPoint coord_next{ coord.x + dx, coord.y };

    dc.DrawLine(coord, coord_next);

    coord = coord_next;

    return coord.x;
}
