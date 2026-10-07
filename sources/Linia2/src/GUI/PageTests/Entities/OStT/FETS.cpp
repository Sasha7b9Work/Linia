// 2026/09/29 11:16:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/FETS.h"
#include "GUI/PageTests/Entities/OStT/FET.h"


FETS::FETS(Test *test) : FET(test)
{

}


void FETS::Draw(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    FET::DrawCommon(dc, c);

    DrawSubstrate(dc, c);
}


void FETS::DrawSubstrate(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    const wxPoint d{ (int)(radius * 0.7), (int)(radius * 0.3) };

    dc.MoveTo({ c.x + d.x, c.y - d.y });
    dc.LineOnDY(2 * d.y);
    dc.MoveTo({ c.x + d.x, c.y });
    dc.LineTo({ c.x + radius, c.y });
    dc.LineOnDX(dr);
    point_S = dc.GetCoord();
}
