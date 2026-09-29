// 2026/09/29 10:56:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/BJTS.h"
#include "GUI/PageTests/Entities/OStT/BJT.h"
#include "Utils/LineDrawer.h"


BJTS::BJTS(Test *test) : OStT4(test)
{

}


void BJTS::Draw(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    int x_vert = 0;
    int y_ground = 0;

    BJT::DrawCommon(dc, c, *this, x_vert, y_ground);

    // Подложка

    int dy = RADIUS * 4 / 16;
    int x = c.x + (c.x - x_vert) + RADIUS / 10;
    LineDriwer driwer( dc, x, c.y - dy );
    driwer.LineToY(c.y + dy);                                   // Вертикальная линия подложки

    {
        // Измеритель подложки

        driwer.MoveTo(x, c.y);
        driwer.LineOnDX(150);

        driwer.MoveOnDX(-100);
        point_substrate = driwer.GetCoord();
        dc.DrawCircle(point_substrate, 5);
        dc.DrawText("Substr", { point_substrate.x - 20, point_substrate.y - 23 });
        driwer.Restore();

        driwer.LineToY(y_ground);
        DrawGround(driwer.GetX(), driwer.GetY(), dc);

        driwer.MoveOnDY(-470);
    }
}
