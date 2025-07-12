// 2025/6/1 17:32:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/Panel11_Menu.h"
#include "MainWindow.h"
#include "Settings/Settings.h"


PanelButtons *PanelButtons::self = nullptr;


enum
{
    ID_BUTTON_FILE = ID_DEFINES_COUNT + 1,
    ID_BUTTON_TEST,
    ID_BUTTON_SETTINGS,
    ID_BUTTON_ARCHIVE,
    ID_BUTTON_GRAPH,
    ID_BUTTON_TABLE,
    ID_BUTTON_REPORT,
    ID_BUTTON_MEASURE,
    ID_BUTTON_DEBUG
};


PanelButtons::PanelButtons(wxWindow* parent) :
    Panel(parent, 0, MainWindow::HEIGHT - HEIGHT, MainWindow::WIDTH1 + MainWindow::WIDTH2, HEIGHT)
{
    self = this;

    wxString labels[100] =
    {
        _L("Файл"),
        _L("Тест"),
        _L("Настройка"),
        _L("Архив"),
        _L("График"),
        _L("Таблица"),
        _L("Отчёт"),
        _L("Измерение"),
        _L("Отладка")
    };


    const int delta = GetSize().x / NumButtons();

    for (int i = 0; i < NumButtons(); i++)
    {
        new wxButton(this, ID_BUTTON_FILE + i, labels[i], {i * delta, 0}, {delta - 3, GetSize().y - 3});
    }

    Bind(wxEVT_BUTTON, &PanelButtons::OnEventButton, this);
}


void PanelButtons::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_BUTTON_FILE)
    {

    }
    else if (id == ID_BUTTON_TEST)
    {

    }
    else if (id == ID_BUTTON_SETTINGS)
    {

    }
    else if (id == ID_BUTTON_ARCHIVE)
    {

    }
    else if (id == ID_BUTTON_GRAPH)
    {

    }
    else if (id == ID_BUTTON_TABLE)
    {

    }
    else if (id == ID_BUTTON_REPORT)
    {

    }
    else if (id == ID_BUTTON_MEASURE)
    {

    }
    else if (id == ID_BUTTON_DEBUG)
    {
        MainWindow::self->SetMode(ModeMainWindow::Debug);
    }
}


int PanelButtons::NumButtons() const
{
//    return SET::GUI::debug_mode.Get() ? 9 : 8;
    return 9;
}
