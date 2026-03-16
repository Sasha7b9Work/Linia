// 2025/6/1 17:32:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/PanelMenu.h"
#include "MainWindow.h"
#include "Settings/Settings.h"
#include "Panels/PanelMenu/DialogModels.h"
#include "Panels/PanelMenu/DialogTest.h"
#include "Panels/PanelMenu/Setup/DialogSetup.h"
#include "Panels/PanelMenu/DialogTable.h"
#include "Panels/PanelMenu/DialogReport.h"


PanelMenu *PanelMenu::self = nullptr;


PanelMenu::PanelMenu(wxWindow* parent) :
    Panel(parent, 0, MainWindow::HEIGHT - HEIGHT, MainWindow::WIDTH_DRAW, HEIGHT)
{
    self = this;

    struct Struct
    {
        wxString label;
        wxButton **button;
    };

    Struct structs[100] =
    {
        { "Модели",    &btnModels },
        { "Тесты",     &btnTests },
        { "Расчёт",    &btnCalculation },
        { "Таблица",   &btnTable },
        { "Отчёт",     &btnReport },
        { "Архив",     &btnArchiv },
        { "Настройки", &btnSetup },
        { "Отладка",   &btnDebug }
    };

    const int delta = Panel::GetSize().x / NumButtons();

    for (int i = 0; i < NumButtons(); i++)
    {
        *structs[i].button = new wxButton(this, wxID_ANY, structs[i].label, {i * delta, 0}, {delta - 3, Panel::GetSize().y - 3});
    }

    Bind(wxEVT_BUTTON, &PanelMenu::OnEventButton, this);
}


void PanelMenu::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnModels->GetId())
    {
        DialogModels().ShowOnWindow(btnModels);
    }
    else if (id == btnTests->GetId())
    {
        DialogTest().ShowOnWindow(btnTests);
    }
    else if (id == btnSetup->GetId())
    {
        DialogSetup().ShowOnWindow(btnSetup);
    }
    else if (id == btnArchiv->GetId())
    {
        MainWindow::self->SetMode(ModeMainWindow::ReferenceGraphs);
    }
    else if (id == btnTable->GetId())
    {
        DialogTable().ShowOnWindow(btnTable);
    }
    else if (id == btnReport->GetId())
    {
        DialogReport().ShowOnWindow(btnReport);
    }
    else if (id == btnCalculation->GetId())
    {
        if (!wndCalculation)
        {
            wndCalculation = new WindowCalculation(MainWindow::self);

            wndCalculation->Bind(wxEVT_CLOSE_WINDOW, [this](wxCloseEvent &event)
                {
                    wndCalculation = nullptr;
                    event.Skip();
                });
        }

        if (wndCalculation)
        {
            wndCalculation->Show();
            wndCalculation->Raise();
        }
    }
    else if (id == btnDebug->GetId())
    {
        MainWindow::self->SetMode(ModeMainWindow::Debug);
    }
}


int PanelMenu::NumButtons() const
{
    return 8;
}
