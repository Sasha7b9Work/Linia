// 2026/09/29 10:56:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/BJTS.h"
#include "GUI/PageTests/Entities/OStT/BJT.h"
#include "Utils/LineDrawer.h"


BJTS::BJTS(Test *test) : BJT(test)
{

}


void BJTS::Draw(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    BJT::DrawCommon(dc, c);

    DrawSubstrate(dc, c);

    FuncAfterDraw(dc);
}


void BJTS::DrawSubstrate(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    const wxPoint d{ (int)(RADIUS * 0.7), (int)(RADIUS * 0.3) };

    LineDriwer driwer{ dc, {c.x + d.x, c.y - d.y} };
    driwer.LineOnDY(2 * d.y);
    driwer.MoveTo(c.x + d.x, c.y);
    driwer.LineTo({ c.x + RADIUS, c.y });
    driwer.LineOnDX(DR);
    point_S = driwer.GetCoord();
}
