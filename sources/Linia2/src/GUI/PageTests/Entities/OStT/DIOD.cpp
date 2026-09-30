// 2026/09/29 11:11:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/DIOD.h"


DIOD::DIOD(Test *test) : OStT(test)
{

}


void DIOD::Draw(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    DrawCommon(dc, c);

    FuncAfterDraw(dc);
}


void DIOD::DrawCommon(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    const wxPoint d = Delta();

    {
        // Рисуем вертикальную линию

        dc.MoveTo({ c.x, c.y - d.y - dr });
        point_C = dc.GetCoord();

        dc.LineTo({ c.x, c.y + d.y + dr });
        point_E = dc.GetCoord();
    }

    {
        // Рисуем перпендикулярные линии

        dc.MoveTo({ c.x - d.x, c.y - d.y });
        dc.LineToX(c.x + d.x);
        dc.MoveToY(c.y + d.y);
        dc.LineToX(c.x - d.x);
    }

    DrawAnode(dc, c);
}


void DIOD::DrawAnode(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    const wxPoint d = Delta();

    if (type == TypeDIOD::Common_Anode_P)
    {
        dc.MoveTo({ c.x - d.x, c.y + d.y });
        dc.LineTo({ c.x, c.y - d.y });
        dc.LineTo({ c.x + d.x, c.y + d.y });
    }
    else if (type == TypeDIOD::Common_Catode_N)
    {
        dc.MoveTo({ c.x - d.x, c.y - d.y });
        dc.LineTo({ c.x, c.y + d.y });
        dc.LineTo({ c.x + d.x, c.y - d.y });
    }
}


wxPoint DIOD::Delta() const
{
    return { (int)(radius * 0.5), (int)(radius * 0.5) };
}
