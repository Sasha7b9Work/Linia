// 2025/7/14 17:22:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Grid.h"


// Сущности для отрисовки - графики, маркеры, еtc.

struct Meas
{
    double u;       // Вольты
    double i;       // Амперы
};


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

    GraphLine(const Meas &_start, const Meas &_end, const wxColor &_color = *wxBLACK) :
        GraphEntity(_color),
        start(_start), end(_end)
    {
    }

    void Draw(const Grid *) const override;

private:

    Meas start;
    Meas end;

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
