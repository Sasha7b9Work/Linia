// 2026/09/29 10:57:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/BJT.h"
#include "Utils/LineDrawer.h"


BJT::BJT(Test *test) : OStT(test)
{

}


void BJT::Draw(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    int x_vert = 0;

    DrawCommon(dc, c, x_vert);

    FuncAfterDraw(dc);
}


void BJT::DrawCommon(wxAutoBufferedPaintDC &dc, const wxPoint &c, int &x_vert)
{
    DrawCase(dc, c);

    DrawBase(dc, c);

    DrawCollector(dc, c);

    FuncAfterDraw(dc);
}


void BJT::DrawBase(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    wxPoint d = DeltaBase();

    wxPoint p1{ c.x - d.x, c.y - d.y };
    wxPoint p2{ c.x - d.x, c.y + d.y };

    dc.DrawLine(p1, p2);

    p1 = { c.x - d.x, c.y };
    p2 = { c.x - RADIUS - DR, c.y };

    point_base = p2;

    dc.DrawLine(p1, p2);
}


void BJT::DrawCollector(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{

}


wxPoint BJT::DeltaBase() const
{
    return { (int)(RADIUS * 0.5), (int)(RADIUS * 0.5) };
}
