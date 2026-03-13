// 2025/7/14 17:22:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/GraphEntity.h"
#include "Display/Display.h"


void GraphLine::Draw(const Grid *grid) const
{
    wxPoint s = grid->ValuesToCoord(start.u, start.i);
    wxPoint e = grid->ValuesToCoord(end.u, end.i);

    Line(s.x, s.y, e.x, e.y).Draw(color);
}


void GraphMeasures::AppendPoint(const wxPoint2DDouble &point)
{
    points.push_back(point);
}


void GraphMeasures::Draw(const Grid *grid) const
{
    Display::self->SetColor(color);

    Spline spline;

    for (auto &coord : points)
    {
        wxPoint point = grid->ValuesToCoord(coord.m_x, coord.m_y);

        spline.AppendPoint({ (double)point.x, (double)point.y });
    }

    spline.Draw(false, true);
}


void GraphEntity::CreateForEmulator(std::vector<GraphEntity *> &entities)
{
    wxColor colors[10] =
    {
        *wxBLACK,
        *wxBLACK,
        *wxBLACK,
        *wxBLACK,
        *wxBLACK,
        *wxBLACK,
        *wxBLACK,
        *wxBLACK,
        *wxBLACK,
        *wxBLACK
    };

    for (int num_line = 0; num_line < 10; num_line++)
    {
        GraphMeasures *meas = new GraphMeasures();

        meas->SetColor(colors[num_line]);

        for (double u = 0.0; u <= 1.0; u += 0.1)
        {
            // Базовый ток пропорционален номеру линии (от 5 до 50 мкА)
            float Ib = 5.0f + (num_line + 1 - 1) * 5.0f; // мкА

            // Коэффициент усиления
            float beta = 8000.0f + (num_line +1 ) * 1000.0f; // немного растет с током

            // Параметры формы кривой
            double U_nas = 30.0;            // напряжение насыщения
            float smoothness = 150.0f;       // плавность перехода
            float Early = 300.0f;            // напряжение Эрли

            // Максимальный ток коллектора
            float Ik_max = beta * Ib * 1e-6f;

            // Плавный переход от 0 к Ik_max с помощью tanh
            float transition = tanhf(smoothness * u / U_nas);

            // Эффект Эрли - небольшой наклон кривых
            float early_effect = 1.0f + u / Early;

            // Небольшая кривизна для реалистичности
            float curvature = 1.0f - 0.05f * expf(-u * 10.0f);

            double i = Ik_max * transition * early_effect * curvature;

            meas->AppendPoint({ u, i });
        }

        entities.push_back(meas);
    }
}
