// 2025/7/14 17:22:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel05_Graph/Grid.h"


// Сущности для отрисовки - графики, маркеры, еtc.


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

    GraphLine(double _x1, double _y1, double _x2, double _y2, const wxColor &_color = *wxBLACK) :
        GraphEntity(_color),
        x1(_x1), y1(_y1), x2(_x2), y2(_y2)
    {
    }

    virtual void Draw(const Grid *) const;

private:

    double x1, y1, x2, y2;

    virtual ~GraphLine() {}
};


class GraphMeasures : public GraphEntity
{
public:

    GraphMeasures(const wxColor &_color) : GraphEntity(_color) { }

    void AppendPoint(const wxPoint2DDouble &);

    virtual void Draw(const Grid *) const;

private:

    std::vector<wxPoint2DDouble> points;
};
