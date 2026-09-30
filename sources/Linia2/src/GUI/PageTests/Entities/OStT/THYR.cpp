// 2026/09/29 11:23:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/THYR.h"


THYR::THYR(Test *test) : DIOD(test)
{

}


void THYR::Draw(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    DIOD::DrawCommon(dc, c);

    DrawControlElectrode(dc, c);

    FuncAfterDraw(dc);
}


void THYR::DrawControlElectrode(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    const wxPoint d = Delta();

    if (DIOD::type == TypeDIOD::Common_Anode_P)
    {
        if (type == TypeTHYR::Control_Anode)
        {
            dc.MoveTo({ c.x, c.y - d.y });

            std::vector<wxPoint> points = IntersectLineCircle(dc.GetCoord(), { c.x - d.x, c.y + d.y }, c, radius);

            dc.LineTo(points[1]);

        }
        else if (type == TypeTHYR::Control_Catode)
        {
            std::vector<wxPoint> points = IntersectLineCircle({ c.x, c.y + d.y }, { c.x - d.x, c.y - d.y }, c, radius);

            dc.MoveTo({ c.x - d.x, c.y - d.y });

            dc.LineTo(points[1]);
        }
    }
    else if (DIOD::type == TypeDIOD::Common_Catode_N)
    {

    }

    dc.LineOnDX(-dr);

    point_B = dc.GetCoord();
}
