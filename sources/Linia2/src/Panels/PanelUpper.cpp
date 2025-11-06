// 2025/6/1 17:14:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelUpper.h"
#include "MainWindow.h"
#include "Utils/GlobalFunctions.h"


PanelUpper *PanelUpper::self = nullptr;


PanelUpper::PanelUpper(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1, 0, MainWindow::WIDTH_DRAW - MainWindow::WIDTH1, MainWindow::HEIGHT_HI)
{
    self = this;

    new wxStaticText(this, wxID_ANY, wxString::Format("ver. %d : %s", VERSION_BUILD, DATE_BUILD), { 460, 56 });

    new wxStaticText(this, wxID_ANY, wxString::Format("IP : %s", GF::GetSelfIP().c_str().AsChar()), {10, 56});
}
