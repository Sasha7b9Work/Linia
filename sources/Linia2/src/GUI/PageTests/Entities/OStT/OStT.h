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
    wxPoint GEtPointGround() const              // Это земля или эмиттер
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
    static const int r = 5;

    // Нарисовать значок земли
    void DrawGround(int x, int y, wxAutoBufferedPaintDC &);

    // Рисует линию длиной length под углом angleDeg
    void DrawLineWithAngle(const wxPoint &start, double length, double angleDeg, wxAutoBufferedPaintDC &);

    // Нарисовать "корпус" транзистора
    void DrawCase(wxAutoBufferedPaintDC &, const wxPoint &c);

private:

    Test *test = nullptr;
};


// ОПИ с двумя выводами


class OStT2 : public OStT
{
public:

    OStT2(Test *test) : OStT(test) { }

private:
};


// ОПИ с тремя выводами


class OStT3 : public OStT
{
public:

    OStT3(Test *test) : OStT(test) { }

    virtual wxPoint GetPointBase(bool &result) const override
    {
        result = true;

        return point_base;
    }

private:
};


// ОПИ с четырьмя выводами


class OStT4 : public OStT
{
public:

    OStT4(Test *test) : OStT(test) { }

    virtual wxPoint GetPointBase(bool &result) const override
    {
        result = true;

        return point_base;
    }

    virtual wxPoint GetPointSubstrate(bool &result) const override
    {
        result = true;

        return point_substrate;
    }

private:
};
