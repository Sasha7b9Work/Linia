// 2025/09/01 22:59:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/PanelErrors.h"
#include "MainWindow.h"


PanelErrors::PanelErrors(wxWindow *parent) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, { MainWindow::WIDTH2, 100 })
{
    text_ctrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, { 0, 0 }, { MainWindow::WIDTH2, 90 }, wxTE_MULTILINE | wxTE_READONLY);

    text_ctrl->SetBackgroundColour({ 255, 0, 0 });

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(text_ctrl, 0, wxEXPAND | wxALL, 0);

    SetSizer(sizer);

    text_ctrl->SetForegroundColour(*wxWHITE);

    text_ctrl->AppendText("ERROR 123 - Мало памяти\n");
    text_ctrl->AppendText("ERROR 177 - Много памяти\n");
}
