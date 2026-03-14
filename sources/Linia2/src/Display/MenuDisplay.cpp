// 2026/03/13 16:31:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/MenuDisplay.h"
#include "Display/Grid/Grid.h"
#include "Display/Display.h"
#include "MainWindow.h"
#include "Settings/Settings.h"


MenuDisplay::MenuDisplay() : wxMenu()
{
    Bind(wxEVT_MENU, &MenuDisplay::OnReset, this, (Append(wxID_ANY, "Сброс"))->GetId());

    wxMenuItem *item = AppendCheckItem(wxID_ANY, "Полный экран");
    Bind(wxEVT_MENU, &MenuDisplay::OnFullScreen, this, item->GetId());
    Check(item->GetId(), Display::self->full_screen);

    AppendSeparator();

    AppendMenuTrack();

    AppendMenuFacade();

    AppendSeparator();

    Bind(wxEVT_MENU, &MenuDisplay::OnSaveGraphic, this, (Append(wxID_ANY, "Сохранить график"))->GetId());
    Bind(wxEVT_MENU, &MenuDisplay::OnResetGraphic, this, (Append(wxID_ANY, "Сбросить график"))->GetId());
}


void MenuDisplay::AppendMenuTrack()
{
    wxMenu *subMenu = new wxMenu();

    wxMenuItem *item = subMenu->AppendCheckItem(wxID_ANY, "X");
    Bind(wxEVT_MENU, &MenuDisplay::OnTrackX, this, item->GetId());
    subMenu->Check(item->GetId(), Display::self->track_x);

    item = subMenu->AppendCheckItem(wxID_ANY, "Y");
    Bind(wxEVT_MENU, &MenuDisplay::OnTrackY, this, item->GetId());
    subMenu->Check(item->GetId(), Display::self->track_y);

    item = subMenu->AppendCheckItem(wxID_ANY, "Ничего");
    Bind(wxEVT_MENU, &MenuDisplay::OnTrackNone, this, item->GetId());
    subMenu->Check(item->GetId(), Display::self->track_none);

    AppendSubMenu(subMenu, "Отслеживать");
}


void MenuDisplay::AppendMenuFacade()
{
    wxMenu *subFacade = new wxMenu();

    {
        // Настройка цветов

        wxMenuItem *item = nullptr;
        StructColor str_color;

#define APPEND_COLOR(title, value_color, func)                          \
        item = subColors->Append(wxID_ANY, title);                      \
        Bind(wxEVT_MENU, &MenuDisplay::OnColor, this, item->GetId());   \
        str_color = { value_color, func };                             \
        colors[item->GetId()] = str_color;

        wxMenu *subColors = new wxMenu();

        APPEND_COLOR("Фон", &SET::GUI::color_background, nullptr);
        APPEND_COLOR("Сетка", &SET::GUI::color_grid, nullptr);
        APPEND_COLOR("Шрифт", &SET::GUI::color_font, nullptr);
        APPEND_COLOR("Кривая", &SET::GUI::color_curve, OnColorCurve);
        APPEND_COLOR("Ссылка", &SET::GUI::color_link, nullptr);
        APPEND_COLOR("Секущая", &SET::GUI::color_secant, nullptr);
        subColors->AppendSeparator();
        APPEND_COLOR("Сбросить", nullptr, nullptr);

        subFacade->AppendSubMenu(subColors, "Цвета");
    }

    Bind(wxEVT_MENU, &MenuDisplay::OnSizePoint, this, (subFacade->Append(wxID_ANY, "Размер точки"))->GetId());

    AppendSubMenu(subFacade, "Внешний вид");
}


void MenuDisplay::OnReset(wxCommandEvent &)
{
    IGrid::self->Reset();
}


void MenuDisplay::OnFullScreen(wxCommandEvent &event)
{
    MainWindow::self->SetMode(event.IsChecked() ? ModeMainWindow::FullGraph : ModeMainWindow::Standard);

    IGrid::self->ResetCenter();

    Display::self->Refresh();
}


void MenuDisplay::OnTrackX(wxCommandEvent &event)
{
    if (event.IsChecked())
    {
        Display::self->track_x = true;
        Display::self->track_y = false;
        Display::self->track_none = false;
    }
}


void MenuDisplay::OnTrackY(wxCommandEvent &event)
{
    if (event.IsChecked())
    {
        Display::self->track_x = false;
        Display::self->track_y = true;
        Display::self->track_none = false;
    }
}


void MenuDisplay::OnTrackNone(wxCommandEvent &event)
{
    if (event.IsChecked())
    {
        Display::self->track_x = false;
        Display::self->track_y = false;
        Display::self->track_none = true;
    }
}


void MenuDisplay::OnColor(wxCommandEvent &event)
{
    auto SetColor = [](const wxString &title, wxColour &color) -> bool
        {
            wxColourData colourData;
            colourData.SetChooseFull(true);

            wxColourDialog dialog(Display::self, &colourData);

            dialog.SetTitle(wxString("Цвет ") + title);

            if (dialog.ShowModal() == wxID_OK)
            {
                color = dialog.GetColourData().GetColour();

                return true;
            }

            return false;
        };

    wxMenuItem *item = FindItem(event.GetId());

    if (item)
    {
        wxColour color;

        auto elem = colors.find(event.GetId());

        if (elem != colors.end())
        {
            if (elem->second.set)
            {
                if (SetColor(item->GetItemLabel(), color))
                {
                    elem->second.set->Set(color.GetRGB());

                    if (elem->second.func)
                    {
                        elem->second.func();
                    }
                }
            }
            else                            // Сбросить
            {
                for (auto col : colors)
                {
                    if (col.second.set)
                    {
                        col.second.set->Reset();
                    }
                }
            }
        }

    }
}


void MenuDisplay::OnSizePoint(wxCommandEvent &)
{

}


void MenuDisplay::OnSaveGraphic(wxCommandEvent &)
{

}


void MenuDisplay::OnResetGraphic(wxCommandEvent &)
{

}


void MenuDisplay::OnColorCurve()
{
    for (auto elem : Display::self->entities)
    {
        elem->SetColor(SET::GUI::color_curve.Get());
    }
}
