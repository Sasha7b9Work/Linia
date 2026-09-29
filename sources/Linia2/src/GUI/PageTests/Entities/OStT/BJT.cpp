// 2026/09/29 10:57:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/BJT.h"
#include "Utils/LineDrawer.h"


BJT::BJT(Test *test) : OStT3(test)
{

}


void BJT::Draw(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    int r = 5;

    int x_col = c.x + RADIUS / 2;   // / Координаты точки коммутации
    int y_col = c.y - 2 * RADIUS;   // / с коллектором

    LineDriwer driwer(dc, x_col, y_col);
    driwer.LineTo(c.x + RADIUS / 2, c.y + 2 * RADIUS);          // Вертикальная линия, которая выходит из коллектора и эмиттера
    DrawGround(driwer.GetX(), driwer.GetY(), dc);
    driwer.MoveOnDY(-20);
    point_emitter = driwer.GetCoord();
    dc.DrawCircle(point_emitter, r);
    dc.DrawText("E", { point_emitter.x + 7, point_emitter.y - 7 });
    dc.DrawCircle(c, RADIUS);
    const int x_vert = c.x - RADIUS * 10 / 18;                        // Здесь заканчивается линия базы внутри окружности
    wxPoint coord_base{ 90, c.y };
    driwer.MoveTo(90, c.y);
    driwer.LineTo(x_vert, c.y);                                             // База
    driwer.MoveOnDX(-50);
    point_base = driwer.GetCoord();
    dc.DrawCircle(point_base, r);
    dc.DrawText("B", { point_base.x - 3, point_base.y - 20 });

    int y_ground = 720;

    {
        // Рисуем транзистор

        {
            // Наклонные линии

            int dy = RADIUS * 4 / 18;

            int y_top = c.y - RADIUS * 100 / 115;
            int y_bottom = c.y + RADIUS * 100 / 115;

            int xx = c.x + RADIUS * 10 / 20;                      // В этом иксе - пересечение коллектора и эмиттера с окружностью.

            dc.DrawLine(x_vert, c.y - dy, xx, y_top);                  // Верхняя наклонная линия (коллектор)
            dc.DrawLine(x_vert, c.y + dy, xx, y_bottom);               // Нижняя наклонная линия (эмиттер)

            {
                // Стрелка эмиттера

                double length = RADIUS * 10 / 40;

                DrawLineWithAngle({ xx, y_bottom }, length, 125, dc);
                DrawLineWithAngle({ xx, y_bottom }, length, 170, dc);
            }
        }

        {
            // Вертикальная линия базы

            int dy = RADIUS * 4 / 9;

            dc.DrawLine(x_vert, c.y - dy, x_vert, c.y + dy);

            {
                // Рисуем измеритель базы

                driwer.MoveTo(coord_base.x, coord_base.y);

                driwer.LineToY(y_ground);

                DrawGround(driwer.GetX(), driwer.GetY(), dc);
            }
        }

        {
            // Подложка

            int dy = RADIUS * 4 / 16;
            int x = c.x + (c.x - x_vert) + RADIUS / 10;
            driwer.MoveTo({ x, c.y - dy });
            driwer.LineToY(c.y + dy);                                   // Вертикальная линия подложки

            {
                // Измеритель подложки

                driwer.MoveTo(x, c.y);
                driwer.LineOnDX(150);

                driwer.MoveOnDX(-100);
                point_substrate = driwer.GetCoord();
                dc.DrawCircle(point_substrate, r);
                dc.DrawText("Substr", { point_substrate.x - 20, point_substrate.y - 23 });
                driwer.Restore();

                driwer.LineToY(y_ground);
                DrawGround(driwer.GetX(), driwer.GetY(), dc);

                driwer.MoveOnDY(-470);
            }
        }
    }

    {
        // Рисуем цепь коллектора

        driwer.MoveTo(x_col, y_col);
        driwer.MoveOnDY(25);
        point_collector = driwer.GetCoord();
        dc.DrawCircle(point_collector, r);
        dc.DrawText("C", { point_collector.x + 7, point_collector.y - 9 });
        driwer.Restore();
        driwer.LineOnDX(355);
        driwer.LineToY(y_ground);
        DrawGround(driwer.GetX(), driwer.GetY(), dc);
    }
}
