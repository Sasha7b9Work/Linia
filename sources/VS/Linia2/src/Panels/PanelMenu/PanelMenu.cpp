// 2025/6/1 17:32:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/PanelMenu.h"
#include "MainWindow.h"
#include "Settings/Settings.h"
#include "Panels/PanelMenu/DialogFile.h"
#include "Panels/PanelMenu/DialogTest.h"
#include "Panels/PanelMenu/Setup/DialogSetup.h"
#include "Panels/PanelMenu/DialogGraph.h"
#include "Panels/PanelMenu/DialogTable.h"
#include "Panels/PanelMenu/DialogReport.h"


PanelButtons *PanelButtons::self = nullptr;


PanelButtons::PanelButtons(wxWindow* parent) :
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
        { "Файл",      &btnFile },
        { "Тест",      &btnTest },
        { "Настройка", &btnSetup },
        { "Архив",     &btnArchiv },
        { "График",    &btnGraph },
        { "Таблица",   &btnTable },
        { "Отчёт",     &btnReport },
        { "Измерение", &btnMeasure },
        { "Отладка",   &btnDebug }
    };

    const int delta = Panel::GetSize().x / NumButtons();

    for (int i = 0; i < NumButtons(); i++)
    {
        *structs[i].button = new wxButton(this, wxID_ANY, structs[i].label, {i * delta, 0}, {delta - 3, Panel::GetSize().y - 3});
    }

    Bind(wxEVT_BUTTON, &PanelButtons::OnEventButton, this);
}


void PanelButtons::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnFile->GetId())
    {
        DialogFile().ShowModal();
    }
    else if (id == btnTest->GetId())
    {
        DialogTest().ShowModal();
    }
    else if (id == btnSetup->GetId())
    {
        DialogSetup().ShowModal();
    }
    else if (id == btnArchiv->GetId())
    {
        MainWindow::self->SetMode(ModeMainWindow::ReferenceGraphs);
    }
    else if (id == btnGraph->GetId())
    {
        DialogGraph().ShowModal();
    }
    else if (id == btnTable->GetId())
    {
        DialogTable().ShowModal();
    }
    else if (id == btnReport->GetId())
    {
        DialogReport().ShowModal();
    }
    else if (id == btnMeasure->GetId())
    {

    }
    else if (id == btnDebug->GetId())
    {
        MainWindow::self->SetMode(ModeMainWindow::Debug);
    }
}


int PanelButtons::NumButtons() const
{
//    return SET::GUI::debug_mode.Get() ? 9 : 8;
    return 9;
}
