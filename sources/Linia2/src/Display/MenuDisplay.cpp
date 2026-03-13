// 2026/03/13 16:31:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/MenuDisplay.h"
#include "Display/Grid.h"
#include "Display/Display.h"
#include "MainWindow.h"


MenuDisplay::MenuDisplay() : wxMenu()
{
    // Добавляем пункты меню
    Append(wxID_RESET, "Сброс");
    itemFullscreen = AppendCheckItem(wxID_ANY, "Полный экран");

    AppendSeparator();

    // Привязываем обработчики для пунктов меню
    Bind(wxEVT_MENU, &MenuDisplay::OnReset, this, wxID_RESET);
    Bind(wxEVT_MENU, &MenuDisplay::OnFullScreen, this, itemFullscreen->GetId());

    Check(itemFullscreen->GetId(), Display::self->full_screen);

    {
        wxMenu *subMenu = new wxMenu();
        itemTrackX = subMenu->AppendCheckItem(wxID_ANY, "X");
        itemTrackY = subMenu->AppendCheckItem(wxID_ANY, "Y");
        itemTrackNone = subMenu->AppendCheckItem(wxID_ANY, "Ничего");

        subMenu->Check(itemTrackX->GetId(), Display::self->track_x);
        subMenu->Check(itemTrackY->GetId(), Display::self->track_y);
        subMenu->Check(itemTrackNone->GetId(), Display::self->track_none);

        AppendSubMenu(subMenu, "Отслеживать");
    }

    {
        wxMenu *subFacade = new wxMenu();

        {
#define APPEND_COLOR(title, item)                                   \
    item = subColors->Append(wxID_ANY, title);                      \
    Bind(wxEVT_MENU, &MenuDisplay::OnColor, this, item->GetId());

            wxMenu *subColors = new wxMenu();

            APPEND_COLOR("Фон", itemColorBackground);
            APPEND_COLOR("Сетка", itemColorGrid);
            APPEND_COLOR("Шрифт", itemColorFont);
            APPEND_COLOR("Кривая", itemColorCurve);
            APPEND_COLOR("Ссылка", itemColorLink);
            APPEND_COLOR("Секущая", itemColorSecant);

            subFacade->AppendSubMenu(subColors, "Цвета");
        }

        AppendSubMenu(subFacade, "Внешний вид");
    }

    Bind(wxEVT_MENU, &MenuDisplay::OnTrackX, this, itemTrackX->GetId());
    Bind(wxEVT_MENU, &MenuDisplay::OnTrackY, this, itemTrackY->GetId());
    Bind(wxEVT_MENU, &MenuDisplay::OnTrackNone, this, itemTrackNone->GetId());
}


void MenuDisplay::OnReset(wxCommandEvent &)
{
    Grid::self->Reset();
}


void MenuDisplay::OnFullScreen(wxCommandEvent &event)
{
    MainWindow::self->SetMode(event.IsChecked() ? ModeMainWindow::FullGraph : ModeMainWindow::Standard);

    Grid::self->ResetCenter();

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
    auto SetColor = []( const wxString &title, wxColour &color) -> bool
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

    int id = event.GetId();

    wxColour color;

    if (id == itemColorBackground->GetId())
    {
        SetColor("Фон", color);
    }
    else if (id == itemColorGrid->GetId())
    {
        SetColor("Сетка", color);
    }
    else if (id == itemColorFont->GetId())
    {
        SetColor("Шрифт", color);
    }
    else if (id == itemColorCurve->GetId())
    {
        SetColor("Кривая", color);
    }
    else if (id == itemColorLink->GetId())
    {
        SetColor("Ссылка", color);
    }
    else if (id == itemColorSecant->GetId())
    {
        SetColor("Секущая", color);
    }
}
