// 2026/09/29 11:03:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/OStT.h"


void OStT::DrawGround(int x, int y, AutoBufferedPaintDC &dc)
{
    dc.DrawLine(x - 10, y, x + 10, y);
}


void OStT::DrawLineWithAngle(const wxPoint &start, double length, double angleDeg, AutoBufferedPaintDC &dc)
{
    double angleRad = angleDeg * M_PI / 180.0;

    int endX = start.x + (int)(length * cos(angleRad));
    int endY = start.y - (int)(length * sin(angleRad));  // минус, т.к. Y вниз

    dc.DrawLine(start.x, start.y, endX, endY);
}


void OStT::DrawCase(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    dc.DrawCircle(c, radius);
}


void OStT::DrawAnchorPoint(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    dc.DrawCircle(c, 5);
}


void OStT::DrawArrow(AutoBufferedPaintDC &dc, const wxPoint &p1, const wxPoint &p2)
{
    double angle = 20.0;

    wxPoint p = PointAtAngle(p1, p2, (double)length_arrow, angle);

    dc.DrawLine(p, p2);

    p = PointAtAngle(p1, p2, (double)length_arrow, -angle);

    dc.DrawLine(p, p2);
}


wxPoint OStT::RotatePoint(const wxPoint &p1, const wxPoint &p2, double angleDeg)
{
    // Вектор от p2 к p1
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;

    // Угол в радианах
    double angleRad = angleDeg * M_PI / 180.0;

    double cosA = std::cos(angleRad);
    double sinA = std::sin(angleRad);

    // Повёрнутый вектор
    double newDx = dx * cosA - dy * sinA;
    double newDy = dx * sinA + dy * cosA;

    // Новая точка
    return wxPoint{
        p2.x + static_cast<int>(newDx + 0.5),
        p2.y + static_cast<int>(newDy + 0.5)
    };
}


wxPoint OStT::PointAtAngle(const wxPoint &p1, const wxPoint &p2, double length, double angleDeg)
{
    // Вектор от p1 к p2 (направление стрелки)
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;

    // Длина вектора
    double len = std::sqrt(dx * dx + dy * dy);
    if (len < 1e-6) return p2;  // защита от нулевой длины

    // Нормализуем
    dx /= len;
    dy /= len;

    // Угол в радианах
    double angleRad = angleDeg * M_PI / 180.0;
    double cosA = std::cos(angleRad);
    double sinA = std::sin(angleRad);

    // Поворачиваем единичный вектор
    double newDx = dx * cosA - dy * sinA;
    double newDy = dx * sinA + dy * cosA;

    // Точка на расстоянии length от p2
    return wxPoint(
        p2.x + static_cast<int>(newDx * length + 0.5),
        p2.y + static_cast<int>(newDy * length + 0.5)
    );
}


std::vector<wxPoint> OStT::IntersectLineCircle(const wxPoint &p1, const wxPoint &p2, const wxPoint &center, double r) const
{
    std::vector<wxPoint> result;

    // Вектор направления прямой
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;

    // Вектор от p1 к центру окружности
    double fx = p1.x - center.x;
    double fy = p1.y - center.y;

    // Коэффициенты квадратного уравнения a*t² + b*t + c = 0
    double a = dx * dx + dy * dy;

    if (a < 1e-9)  // p1 и p2 совпадают — прямая вырождена
    {
        return result;
    }

    double b = 2.0 * (fx * dx + fy * dy);
    double c = fx * fx + fy * fy - r * r;

    // Дискриминант
    double discriminant = b * b - 4.0 * a * c;

    if (discriminant < 0)
    {
        // Нет пересечений
        return result;
    }

    double sqrtD = std::sqrt(discriminant);

    // Первая точка
    double t1 = (-b - sqrtD) / (2.0 * a);
    result.emplace_back(wxPoint(
        static_cast<int>(p1.x + t1 * dx + 0.5),
        static_cast<int>(p1.y + t1 * dy + 0.5)
    ));

    // Вторая точка (если дискриминант > 0)
    if (discriminant > 1e-9)
    {
        double t2 = (-b + sqrtD) / (2.0 * a);
        result.emplace_back(wxPoint(
            static_cast<int>(p1.x + t2 * dx + 0.5),
            static_cast<int>(p1.y + t2 * dy + 0.5)
        ));
    }

    return result;
}


void OStT::FuncAfterDraw(AutoBufferedPaintDC &dc)
{
    DrawAnchorPoints(dc);
}


void OStT::DrawAnchorPoints(AutoBufferedPaintDC &dc)
{
    DrawAnchorPoint(dc, GetPoint_C());

    DrawAnchorPoint(dc, GetPoint_E());

    wxPoint point;

    if (GetPoint_B(point))
    {
        DrawAnchorPoint(dc, point);
    }
    if (GetPoint_S(point))
    {
        DrawAnchorPoint(dc, point);
    }
}
