// 2026/3/15 19:49:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Graphics/AutoCursors.h"
#include "Display/Grid/GridNew.h"
#include "Display/Display.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/FinderMinMax.h"


void AutoCursors::Draw(const std::vector<GraphMeasure *> &measures)
{
    wxPoint mouse_pos = Grid::self->GetMousePosition();

    wxPoint2DDouble value = Grid::self->CoordToValues(mouse_pos);

    wxRect rect = Grid::self->GetRect();

    Display::self->SetColorPen(SET::GUI::color_curve.Get());

    if (SET::GUI::track_y.Get())
    {
        FinderMinMax <int>finder(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

        for (GraphMeasure *meas : measures)
        {
            auto result = Math::GetIntersectionY(meas->rel_points, mouse_pos.y);

            if (result.second)
            {
                Line(result.first.x, rect.GetTop(), result.first.x, rect.GetBottom()).Draw();

                finder.Push(result.first.x);
            }
        }

        finder.Push(mouse_pos.x);

        Line(finder.Min(), mouse_pos.y, finder.Max(), mouse_pos.y).Draw(SET::GUI::color_curve.Get());
    }

    if (SET::GUI::track_x.Get())
    {
        FinderMinMax <int>finder(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

        for (GraphMeasure *meas : measures)
        {
            auto result = Math::GetIntersectionX(meas->rel_points, mouse_pos.x);

            if (result.second)
            {
                Line(rect.GetLeft(), result.first.y, rect.GetRight(), result.first.y).Draw();

                finder.Push(result.first.y);
            }
        }

        finder.Push(mouse_pos.y);

        Line(mouse_pos.x, finder.Min(), mouse_pos.x, finder.Max()).Draw(SET::GUI::color_curve.Get());
    }

    if (SET::GUI::track_mouse)
    {
        Text::SetFont();
        Text(wxString::Format("%.1f : %.1f", value.m_x, -value.m_y)).DrawAboutRightUp(mouse_pos.x + 5, mouse_pos.y - 5, true, true);
    }
}
