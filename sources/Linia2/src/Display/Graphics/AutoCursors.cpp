// 2026/3/15 19:49:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Graphics/AutoCursors.h"
#include "Display/Grid/GridNew.h"
#include "Display/Display.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


void AutoCursors::Draw(const std::vector<GraphMeasure *> &measures)
{
    Text::SetFont();

    wxPoint mouse_pos = Grid::self->GetMousePosition();

    wxPoint2DDouble value = Grid::self->CoordToValues(mouse_pos);

    Text(wxString::Format("%.1f : %.1f", value.m_x, -value.m_y)).DrawAboutRightUp(mouse_pos.x + 5, mouse_pos.y - 5, true);

    wxRect rect = Grid::self->GetRect();

    if (Display::self->track_y)
    {
        Line(rect.GetLeft(), mouse_pos.y, rect.GetRight(), mouse_pos.y).Draw(SET::GUI::color_curve.Get());
    }

    if (Display::self->track_x)
    {
        Line(mouse_pos.x, rect.GetTop(), mouse_pos.x, rect.GetBottom()).Draw(SET::GUI::color_curve.Get());

        for (GraphMeasure *meas : measures)
        {
            auto result = Math::GetIntersectionX(meas->rel_points, mouse_pos.x);

            if (result.second)
            {
                Line(rect.GetLeft(), result.first.y, rect.GetRight(), result.first.y).Draw();
            }
        }
    }
}
