// 2026/09/29 11:08:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/CAP.h"


CAP::CAP(Test *test) : OStT(test)
{

}


void CAP::Draw(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    const wxPoint d{ (int)(radius * 0.8), (int)(radius * 0.15) };

    dc.DrawLine({ c.x - d.x, c.y - d.y }, { c.x + d.x, c.y - d.y });

    dc.DrawLine({ c.x - d.x,c.y + d.y }, { c.x + d.x, c.y + d.y });

    {
        // Вывод коллектора

        dc.MoveTo({ c.x, c.y - d.y });

        dc.LineToY(c.y - radius);

        dc.LineOnDY(-dr);

        point_C = dc.GetCoord();
    }

    {
        // Вывод эмиттера

        dc.MoveTo({ c.x, c.y + d.y });

        dc.LineToY(c.y + radius);

        dc.LineOnDY(dr);

        point_E = dc.GetCoord();
    }

    FuncAfterDraw(dc);
}
