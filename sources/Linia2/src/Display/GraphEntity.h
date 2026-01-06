// 2025/7/14 17:22:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Grid.h"


// Сущности для отрисовки - графики, маркеры, еtc.


// Единицы измерения - вольты, амперы

enum TypeUnit
{
    A,
    V
};

struct Unit
{
    Unit(double v, TypeUnit t) : val(v), type(t) { }
    double val;
    TypeUnit type;
};


struct UPoint
{
    UPoint(const Unit &_x, const Unit &_y) : x(_x), y(_y) { }
    Unit x;
    Unit y;
};


#define VOLT(x) Unit(x, V)
#define AMPS(x) Unit(x, A)


class GraphEntity
{
public:

    GraphEntity(const wxColor &_color = *wxBLACK) : color(_color) { }

    virtual void Draw(const Grid *) const = 0;

protected:

    virtual ~GraphEntity() { }

    wxColor color;
};


class GraphLine : public GraphEntity
{
public:

    GraphLine(const UPoint &_start, const UPoint &_end, const wxColor &_color = *wxBLACK) :
        GraphEntity(_color),
        start(_start), end(_end)
    {
    }

    void Draw(const Grid *) const override;

private:

    UPoint start;
    UPoint end;

    virtual ~GraphLine() {}
};


class GraphMeasures : public GraphEntity
{
public:

    GraphMeasures(const wxColor &_color) : GraphEntity(_color) { }

    void AppendPoint(const wxPoint2DDouble &);

    void Draw(const Grid *) const override;

private:

    std::vector<wxPoint2DDouble> points;    // Единицы здесь вольты, амперы
};
