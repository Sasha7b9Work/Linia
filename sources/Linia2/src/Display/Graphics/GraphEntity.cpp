// 2025/7/14 17:22:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Graphics/GraphEntity.h"
#include "Display/Display.h"
#include "Settings/Settings.h"


void GraphLine::Draw(const IGrid *grid) const
{
    wxPoint s = grid->ValuesToCoord(start.u, start.i);
    wxPoint e = grid->ValuesToCoord(end.u, end.i);

    Line(s.x, s.y, e.x, e.y).Draw(color);
}


void GraphMeasures::AppendPoint(const wxPoint2DDouble &point)
{
    points.push_back(point);
}


void GraphMeasures::Draw(const IGrid *grid) const
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
    for (int num_line = 0; num_line < 10; num_line++)
    {
        GraphMeasures *meas = new GraphMeasures();

        meas->SetColor(SET::GUI::color_curve.Get());

        for (double u = 0.0; u <= 1.0; u += 0.1)
        {
            // Базовый ток пропорционален номеру линии (от 5 до 50 мкА)
            double Ib = 5.0 + (num_line + 1 - 1) * 5.0; // мкА

            // Коэффициент усиления
            double beta = 8000.0 + (num_line + 1) * 1000.0; // немного растет с током

            // Параметры формы кривой
            double U_nas = 30.0;                // напряжение насыщения
            double smoothness = 150.0f;         // плавность перехода
            double Early = 300.0;               // напряжение Эрли

            // Максимальный ток коллектора
            double Ik_max = beta * Ib * 1e-6;

            // Плавный переход от 0 к Ik_max с помощью tanh
            double transition = tanh(smoothness * u / U_nas);

            // Эффект Эрли - небольшой наклон кривых
            double early_effect = 1.0 + u / Early;

            // Небольшая кривизна для реалистичности
            double curvature = 1.0 - 0.05 * exp(-u * 10.0);

            double i = Ik_max * transition * early_effect * curvature;

            meas->AppendPoint({ u, i });
        }

        entities.push_back(meas);
    }
}
