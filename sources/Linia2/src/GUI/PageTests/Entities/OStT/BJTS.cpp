// 2026/09/29 10:56:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/BJTS.h"
#include "GUI/PageTests/Entities/OStT/BJT.h"


BJTS::BJTS() : BJT()
{

}


void BJTS::Draw(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    BJT::DrawCommon(dc, c, true);

    DrawSubstrate(dc, c);
}


void BJTS::DrawSubstrate(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    const wxPoint d{ (int)(radius * 0.7), (int)(radius * 0.3) };

    dc.MoveTo({ c.x + d.x, c.y - d.y });
    dc.LineOnDY(2 * d.y);
    dc.MoveTo({ c.x + d.x, c.y });
    dc.LineTo({ c.x + radius, c.y });
    dc.LineOnDX(dr);
    point_S = dc.GetCoord();
}
