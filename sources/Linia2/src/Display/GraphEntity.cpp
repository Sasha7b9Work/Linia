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

        for (double u = 0.0; u <= 1.0; u += 0.05)
        {
            // Базовый ток пропорционален номеру линии
            double Ib = (double)(num_line + 1) * 10.0; // мкА

            // Параметры транзистора (подобраны для красивого графика)
            double beta = 9000.0;        // коэффициент усиления
            double Upor = 0.3;            // пороговое напряжение (напряжение насыщения)
            double Uearly = 50.0;         // напряжение Эрли для наклона кривых

            double i = 0.0;

            // Если напряжение меньше порогового - линейная область (насыщение)
            if (u < Upor)
            {
                // В области насыщения ток растет линейно от 0 до максимального
                double Ik_max = beta * Ib * 1e-6f; // максимальный ток для этой линии
                i = Ik_max * (u / Upor) * (1.0f + (u - Upor) / Uearly);
            }
            else
            {
                // Активная область - ток слабо растет с напряжением из-за эффекта Эрли
                double Ik = beta * Ib * 1e-6f; // базовый ток
                // Добавляем небольшой рост с напряжением и небольшую кривизну
                double Early_factor = 1.0f + (u - Upor) / Uearly;
                // Добавляем небольшую нелинейность для большей реалистичности
                double nonlinearity = 1.0f + 0.02f * sin(u * 3.14159f);
                i = Ik * Early_factor * nonlinearity;
            }

            meas->AppendPoint({ u, i });
        }

        entities.push_back(meas);
    }
}
