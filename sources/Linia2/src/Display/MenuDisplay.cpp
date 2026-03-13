// 2026/03/13 16:31:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/MenuDisplay.h"
#include "Display/Grid.h"
#include "Display/Display.h"
#include "MainWindow.h"


MenuDisplay::MenuDisplay() : wxMenu()
{
    wxMenuItem *item = nullptr;

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

        item = subMenu->AppendCheckItem(wxID_ANY, "X");
        Bind(wxEVT_MENU, &MenuDisplay::OnTrackX, this, item->GetId());
        subMenu->Check(item->GetId(), Display::self->track_x);

        itemTrackY = subMenu->AppendCheckItem(wxID_ANY, "Y");
        itemTrackNone = subMenu->AppendCheckItem(wxID_ANY, "Ничего");

        subMenu->Check(itemTrackY->GetId(), Display::self->track_y);
        subMenu->Check(itemTrackNone->GetId(), Display::self->track_none);

        AppendSubMenu(subMenu, "Отслеживать");
    }

    {
        wxMenu *subFacade = new wxMenu();

        {
            // Настройка цветов

#define APPEND_COLOR(title) Bind(wxEVT_MENU, &MenuDisplay::OnColor, this, (subColors->Append(wxID_ANY, title))->GetId());

            wxMenu *subColors = new wxMenu();

            APPEND_COLOR("Фон");
            APPEND_COLOR("Сетка");
            APPEND_COLOR("Шрифт");
            APPEND_COLOR("Кривая");
            APPEND_COLOR("Ссылка");
            APPEND_COLOR("Секущая");

            subFacade->AppendSubMenu(subColors, "Цвета");
        }

        Bind(wxEVT_MENU, &MenuDisplay::OnSizePoint, this, (subFacade->Append(wxID_ANY, "Размер точки"))->GetId());

        AppendSubMenu(subFacade, "Внешний вид");
    }

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

    wxMenuItem *item = FindItem(event.GetId());

    if (item)
    {
        wxColour color;

        SetColor(item->GetItemLabel(), color);
    }
}


void MenuDisplay::OnSizePoint(wxCommandEvent &)
{

}
