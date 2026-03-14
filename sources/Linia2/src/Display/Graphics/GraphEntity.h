// 2025/7/14 17:22:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Grid/Grid.h"


// Сущности для отрисовки - графики, маркеры, еtc.

struct Meas
{
    double u;       // Вольты
    double i;       // Амперы
};


class GraphEntity
{
public:

    GraphEntity() : color{ *wxWHITE } { }

    virtual void Draw(const IGrid *) = 0;

    void SetColor(const wxColor &_color) { color = _color; }

    static void CreateForEmulator(std::vector<GraphEntity *> &);

protected:

    virtual ~GraphEntity() { }

    wxColor color;
};


class GraphLine : public GraphEntity
{
public:

    GraphLine(const Meas &_start, const Meas &_end) :
        GraphEntity(),
        start(_start), end(_end)
    {
    }

    void Draw(const IGrid *) override;

private:

    Meas start;
    Meas end;

    virtual ~GraphLine() {}
};


class GraphMeasures : public GraphEntity
{
public:

    GraphMeasures() : GraphEntity() { }

    void AppendPoint(const wxPoint2DDouble &);

    void Draw(const IGrid *) override;

private:

    std::vector<wxPoint2DDouble> abs_points;    // В абсолютных значениях - вольты, амперы
    std::vector<wxPoint> rel_points;            // А здесь уже значения в координатах экрана, готовые к отрисовке
};
