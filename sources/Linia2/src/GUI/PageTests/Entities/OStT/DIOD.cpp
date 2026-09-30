// 2026/09/29 11:11:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/DIOD.h"
#include "Utils/LineDrawer.h"


DIOD::DIOD(Test *test) : OStT(test)
{

}


void DIOD::Draw(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    DrawCase(dc, c);

    DrawCommon(dc, c);

    FuncAfterDraw(dc);
}


void DIOD::DrawCommon(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    {
        // Рисуем вертикальную линию

        LineDriwer driwer{ dc, c };

        driwer.MoveOnDY(-radius - dr);

        point_C = driwer.GetCoord();

        driwer.LineOnDY(dr * 2 + radius * 2);

        point_E = driwer.GetCoord();
    }

    {
        // Рисуем перпендикулярные линии

        wxPoint d = Delta();

        LineDriwer driwer{ dc, {c.x - d.x, c.y - d.y} };

        driwer.LineToX(c.x + d.x);

        driwer.MoveToY(c.y + d.y);

        driwer.LineToX(c.x - d.x);
    }
}


wxPoint DIOD::Delta() const
{
    return { (int)(radius * 0.5), (int)(radius * 0.5) };
}
