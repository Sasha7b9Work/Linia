// 2026/09/29 11:10:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/DARL.h"


DARL::DARL(Test *test) : BJT(test)
{

}


void DARL::Draw(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    DrawCase(dc, c, RADIUS);

    int dx = (int)(RADIUS * 0.3);
    int dy = (int)(RADIUS * 0.3);

    int r = RADIUS / 4;

    DrawCommon(dc, { c.x - dx, c.y - dy }, false, r);

    DrawCommon(dc, { c.x + dx, c.y + dy }, false, r);
}
