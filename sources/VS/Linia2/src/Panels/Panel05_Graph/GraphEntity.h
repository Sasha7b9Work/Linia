// 2025/7/14 17:22:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel05_Graph/Grid.h"


// Сущности для отрисовки - графики, маркеры, еtc.


class GraphEntity
{
public:

    virtual void Draw(const Grid *) const = 0;

protected:

    virtual ~GraphEntity() { }
};


class GraphLine : public GraphEntity
{
public:

    GraphLine(double _x1, double _y1, double _x2, double _y2) :
        x1(_x1), y1(_y1), x2(_x2), y2(_y2)
    {
    }


    virtual void Draw(const Grid *) const;

private:

    double x1, y1, x2, y2;

    virtual ~GraphLine() {}
};
