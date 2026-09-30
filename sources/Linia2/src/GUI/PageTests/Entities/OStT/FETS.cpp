// 2026/09/29 11:16:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/FETS.h"
#include "GUI/PageTests/Entities/OStT/FET.h"
#include "Utils/LineDrawer.h"


FETS::FETS(Test *test) : FET(test)
{

}


void FETS::Draw(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    FET::DrawCommon(dc, c);

    DrawSubstrate(dc, c);

    FuncAfterDraw(dc);
}


void FETS::DrawSubstrate(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    const wxPoint d{ (int)(radius * 0.7), (int)(radius * 0.3) };

    LineDriwer driwer{ dc, {c.x + d.x, c.y - d.y} };
    driwer.LineOnDY(2 * d.y);
    driwer.MoveTo(c.x + d.x, c.y);
    driwer.LineTo({ c.x + radius, c.y });
    driwer.LineOnDX(DR);
    point_S = driwer.GetCoord();
}
