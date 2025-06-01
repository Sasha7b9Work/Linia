// 2025/6/1 17:32:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Buttons.h"
#include "MainWindow.h"


enum
{
    ID_BUTTON_FILE = ID_DEFINES_COUNT + 1,
    ID_BUTTON_TEST,
    ID_BUTTON_SETTINGS,
    ID_BUTTON_ARCHIVE,
    ID_BUTTON_GRAPH,
    ID_BUTTON_TABLE,
    ID_BUTTON_REPORT,
    ID_BUTTON_MEASURE
};


PanelButtons::PanelButtons(wxWindow* parent) :
    Panel(parent, 0, MainWindow::HEIGHT - HEIGHT, MainWindow::WIDTH1 + MainWindow::WIDTH2, HEIGHT)
{
    volatile wxButton *buttons[NUM_BUTTONS];

    buttons[0] = btnFile;
    buttons[1] = btnTest;
    buttons[2] = btnSettings;
    buttons[3] = btnArchive;
    buttons[4] = btnGraph;
    buttons[5] = btnTable;
    buttons[6] = btnReport;
    buttons[7] = btnMeasure;

    int delta = GetSize().x / NUM_BUTTONS;

    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        buttons[i] = new wxButton(this, ID_BUTTON_FILE + 1, "", { i * delta, 0 }, { delta, GetSize().y });
    }

    buttons[0] = buttons[0];
}
