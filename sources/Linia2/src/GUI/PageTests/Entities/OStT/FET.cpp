// 2026/09/29 11:15:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/FET.h"


FET::FET(Test *test) : OStT3(test)
{

}


void FET::Draw(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    DrawCommon(dc, c, *this);
}


void FET::DrawCommon(wxAutoBufferedPaintDC &dc, const wxPoint &c, OStT &self)
{
    self.DrawCase(dc, c);

    DrawGate(dc, c, self);

    DrawSourceDrain(dc, c, self);
}


void FET::DrawGate(wxAutoBufferedPaintDC &dc, const wxPoint &c, OStT &self)
{
    wxPoint p1{ c.x - GateDX(), c.y - GateDY() };
    wxPoint p2{ c.x - GateDX(), c.y + GateDY() };

    dc.DrawLine(p1, p2);

    p1 = wxPoint{ c.x - GateDX(), c.y + DrainDY() };
    p2 = wxPoint{ c.x - RADIUS - DR, c.y + DrainDY() };

    dc.DrawLine(p1, p2);

    self.DrawAnchorPoint(dc, p2);
}


void FET::DrawSourceDrain(wxAutoBufferedPaintDC &dc, const wxPoint &c, OStT &self)
{
    int dx = (int)std::sqrt(RADIUS * RADIUS - DrainDY() * DrainDY());

    wxPoint p1{ c.x, c.y - DrainDY() };
    wxPoint p2{ c.x + dx + DR, c.y - DrainDY() };

    dc.DrawLine(p1, p2);

    self.DrawAnchorPoint(dc, p2);

    p1.y = c.y + DrainDY();
    p2.y = p1.y;

    dc.DrawLine(p1, p2);

    self.DrawAnchorPoint(dc, p2);
}


int FET::GateDX()
{
    return 0;
}


int FET::GateDY()
{
    return (int)(RADIUS * 0.8f);
}


int FET::DrainDY()
{
    return (int)(RADIUS * 0.55f);
}
