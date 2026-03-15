// 2026/3/15 19:49:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Graphics/AutoCursors.h"
#include "Display/Grid/GridNew.h"
#include "Display/Display.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


void AutoCursors::Draw(const std::vector<GraphMeasure *> &measures)
{
    wxPoint mouse_pos = Grid::self->GetMousePosition();

    wxPoint2DDouble value = Grid::self->CoordToValues(mouse_pos);

    wxRect rect = Grid::self->GetRect();

    Display::self->SetColorPen(SET::GUI::color_grid.Get());

    if (SET::GUI::track_y.Get())
    {
        for (GraphMeasure *meas : measures)
        {
            auto result = Math::GetIntersectionY(meas->rel_points, mouse_pos.y);

            if (result.second)
            {
                Line(result.first.x, rect.GetTop(), result.first.x, rect.GetBottom()).Draw();
            }
        }
    }

    if (SET::GUI::track_x.Get())
    {
        for (GraphMeasure *meas : measures)
        {
            auto result = Math::GetIntersectionX(meas->rel_points, mouse_pos.x);

            if (result.second)
            {
                Line(rect.GetLeft(), result.first.y, rect.GetRight(), result.first.y).Draw();
            }
        }
    }

    Text::SetFont();
    Text(wxString::Format("%.1f : %.1f", value.m_x, -value.m_y)).DrawAboutRightUp(mouse_pos.x + 5, mouse_pos.y - 5, true);
}
