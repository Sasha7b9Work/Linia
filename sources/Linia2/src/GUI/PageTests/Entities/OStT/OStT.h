// 2026/09/29 11:03:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#pragma warning(push, 0)
    #include <wx/dcbuffer.h>
#pragma warning(pop)


// Object Subject to Testing - ОПИ - объект, подлежащий исследованию


class Test;


class OStT
{
public:

    virtual ~OStT() { }

    static const int RADIUS = 50;

    virtual void Draw(wxAutoBufferedPaintDC &, const wxPoint &) = 0;

    // Точка привязки базы
    virtual wxPoint GetPointBase(bool &result) const
    {
        result = false;

        return point_base;
    }

    // Точка привязки коллектора
    wxPoint GetPointCollector() const
    {
        return point_collector;
    }

    // Точка привязки подложки
    virtual wxPoint GetPointSubstrate(bool &result) const
    {
        result = false;

        return point_substrate;
    }

    // Точка привязки земли/эмиттера
    wxPoint GetPointGround() const              // Это земля или эмиттер
    {
        return point_emitter;
    }

protected:

    friend class BJT;
    friend class FET;

    OStT(Test *_test) : test(_test) { }

    wxPoint point_emitter;
    wxPoint point_collector;
    wxPoint point_base;
    wxPoint point_substrate;

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
