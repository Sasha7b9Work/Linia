// 2026/09/29 11:03:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "Utils/AutoBufferedPaintDC.h"


// Object Subject to Testing - ОПИ - объект, подлежащий исследованию


class Test;


/*
*   Точки подключения: 
*   База (B) - база, затвор
*   Коллектор (C) - коллектор, сток
*   Эмиттер (E) - эмиттер, исток
*   Подложка (S)
*/


struct StoredValue
{
    StoredValue(int def) : value{ def } { }

    int value = 0;

    void Store()
    {
        stored = value;
    }

    void Restore()
    {
        value = stored;
    }

    operator int() const
    {
        return value;
    }

private:

    int stored = 0;
};


class OStT
{
public:

    virtual ~OStT() { }

    virtual void Draw(AutoBufferedPaintDC &, const wxPoint &) = 0;

    // Точка привязки базы
    virtual bool GetPoint_B(wxPoint &result) const
    {
        result = point_B;

        return false;
    }

    // Точка привязки коллектора
    wxPoint GetPoint_C() const
    {
        return point_C;
    }

    // Точка привязки подложки
    virtual bool GetPoint_S(wxPoint &result) const
    {
        result = point_S;

        return false;
    }

    // Точка привязки земли/эмиттера
    wxPoint GetPoint_E() const              // Это земля или эмиттер
    {
        return point_E;
    }

    virtual wxString GetName_B() const
    {
        return "B";
    }

    virtual wxString GetName_C() const
    {
        return "C";
    }

    virtual wxString GetName_E() const
    {
        return "E";
    }

    virtual wxString GetName_S() const
    {
        return "Substr";
    }

    void FuncAfterDraw(AutoBufferedPaintDC &);

protected:

    friend class BJT;
    friend class FET;

    OStT(Test *_test) : test(_test) { }

    wxPoint point_E;
    wxPoint point_C;
    wxPoint point_B;
    wxPoint point_S;

    StoredValue radius{ 50 };               // Радиус корпуса. Может изменяться
    StoredValue dr{ 40 };                   // На столько пикселей выступает точка привязки за окружность корпуса
    StoredValue length_arrow{ 15 };         // Длина стрелки на эмиттере

    static const int y_ground = 720;        // Координата y отрисовки земли

    // Рисует линию длиной length под углом angleDeg
    void DrawLineWithAngle(const wxPoint &start, double length, double angleDeg, AutoBufferedPaintDC &);

    // Нарисовать "корпус" транзистора
    void DrawCase(AutoBufferedPaintDC &, const wxPoint &c);

    // Нарисовать точку привязки
    void DrawAnchorPoint(AutoBufferedPaintDC &, const wxPoint &);

    void DrawAnchorPoints(AutoBufferedPaintDC &);

    void DrawNamesPoints(AutoBufferedPaintDC &);

    // Нарисовать стрелку из точки 1 в точку 2
    void DrawArrow(AutoBufferedPaintDC &, const wxPoint &p1, const wxPoint &p2);

    // Повернуть точку p1 вокруг точки p2 на угол angleDeg (в градусах)
    wxPoint RotatePoint(const wxPoint &p1, const wxPoint &p2, double angleDeg);

    // p1, p2 — точки, задающие направление
    // length — длина стрелки (перьев)
    // angleDeg — угол отклонения от направления p1->p2 (в градусах)
    // Возвращает точку на расстоянии length от p2, повёрнутую на angleDeg
    wxPoint PointAtAngle(const wxPoint &p1, const wxPoint &p2, double length, double angleDeg);

    // Находит точки пересечения прямой (p1, p2) с окружностью (center, radius)
    // Возвращает вектор точек пересечения (0, 1 или 2 точки)
    std::vector<wxPoint> IntersectLineCircle(const wxPoint &p1, const wxPoint &p2, const wxPoint &center, double radius) const;

private:

    Test *test = nullptr;
};
