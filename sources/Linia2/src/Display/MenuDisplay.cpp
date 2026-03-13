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
            // Настройка цветов

#define APPEND_COLOR(title)                                         \
    item = subColors->Append(wxID_ANY, title);                      \
    Bind(wxEVT_MENU, &MenuDisplay::OnColor, this, item->GetId());

            wxMenu *subColors = new wxMenu();

            wxMenuItem *item = nullptr;
            APPEND_COLOR("Фон");
            APPEND_COLOR("Сетка");
            APPEND_COLOR("Шрифт");
            APPEND_COLOR("Кривая");
            APPEND_COLOR("Ссылка");
            APPEND_COLOR("Секущая");

            subFacade->AppendSubMenu(subColors, "Цвета");
        }

        itemSizePoint = subFacade->Append(wxID_ANY, "Размер точки");
        Bind(wxEVT_MENU, &MenuDisplay::OnSizePoint, this, itemSizePoint->GetId());

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
