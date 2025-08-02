// 2025/6/1 17:32:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/Panel11_Menu.h"
#include "MainWindow.h"
#include "Settings/Settings.h"
#include "Panels/Panel11_Menu/DialogFile.h"
#include "Panels/Panel11_Menu/DialogTest.h"
#include "Panels/Panel11_Menu/Setup/DialogSetup.h"
#include "Panels/Panel11_Menu/DialogGraph.h"
#include "Panels/Panel11_Menu/DialogTable.h"
#include "Panels/Panel11_Menu/DialogReport.h"


PanelButtons *PanelButtons::self = nullptr;


PanelButtons::PanelButtons(wxWindow* parent) :
    Panel(parent, 0, MainWindow::HEIGHT - HEIGHT, MainWindow::WIDTH1 + MainWindow::WIDTH2, HEIGHT)
{
    self = this;

    struct Struct
    {
        wxString label;
        wxButton *button;
    };

    Struct structs[100] =
    {
        { _L("Файл"),      btnFile },
        { _L("Тест"),      btnTest },
        { _L("Настройка"), btnSetup },
        { _L("Архив"),     btnArchiv },
        { _L("График"),    btnGraph },
        { _L("Таблица"),   btnTable },
        { _L("Отчёт"),     btnReport },
        { _L("Измерение"), btnMeasure },
        { _L("Отладка"),   btnDebug }
    };

    const int delta = GetSize().x / NumButtons();

    for (int i = 0; i < NumButtons(); i++)
    {
        structs[i].button = new wxButton(this, wxID_ANY, structs[i].label, {i * delta, 0}, {delta - 3, GetSize().y - 3});
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
