// 2026/09/29 11:03:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#pragma warning(push, 0)
    #include <wx/dcbuffer.h>
#pragma warning(pop)


// Object Subject to Testing - ОПИ - объект, подлежащий исследованию


class Test;


/*
*   Точки подключения: 
*   База (B) - база, затвор
*   Коллектор (C) - коллектор, сток
*   Эмиттер (E) - эмиттер, исток
*   Подложка (S)
*/


class OStT
{
public:

    virtual ~OStT() { }

    static const int RADIUS = 50;

    virtual void Draw(wxAutoBufferedPaintDC &, const wxPoint &) = 0;

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

protected:

    friend class BJT;
    friend class FET;

    OStT(Test *_test) : test(_test) { }

    wxPoint point_E;
    wxPoint point_C;
    wxPoint point_B;
    wxPoint point_S;

    static const int y_ground = 720;        // Координата y отрисовки земли
    static const int DR = 40;               // На столько пикселей выступает точка привязки за окружность корпуса

    void FuncAfterDraw(wxAutoBufferedPaintDC &);

    // Нарисовать значок земли
    void DrawGround(int x, int y, wxAutoBufferedPaintDC &);

    // Рисует линию длиной length под углом angleDeg
    void DrawLineWithAngle(const wxPoint &start, double length, double angleDeg, wxAutoBufferedPaintDC &);

    // Нарисовать "корпус" транзистора
    void DrawCase(wxAutoBufferedPaintDC &, const wxPoint &c);

    // Нарисовать точку привязки
    void DrawAnchorPoint(wxAutoBufferedPaintDC &, const wxPoint &);

    void DrawAnchorPoints(wxAutoBufferedPaintDC &);

    // Нарисовать стрелку из точки 1 в точку 2
    void DrawArrow(wxAutoBufferedPaintDC &, const wxPoint &p1, const wxPoint &p2);

    // Повернуть точку p1 вокруг точки p2 на угол angleDeg (в градусах)
    wxPoint RotatePoint(const wxPoint &p1, const wxPoint &p2, double angleDeg);

    // p1, p2 — точки, задающие направление
    // length — длина стрелки (перьев)
    // angleDeg — угол отклонения от направления p1->p2 (в градусах)
    // Возвращает точку на расстоянии length от p2, повёрнутую на angleDeg
    wxPoint PointAtAngle(const wxPoint &p1, const wxPoint &p2, double length, double angleDeg);

    // Находит точки пересечения прямой (p1, p2) с окружностью (center, radius)
    // Возвращает вектор точек пересечения (0, 1 или 2 точки)
    std::vector<wxPoint> IntersectLineCircle(const wxPoint &p1, const wxPoint &p2, const wxPoint &center, double radius);

private:

    Test *test = nullptr;
};
