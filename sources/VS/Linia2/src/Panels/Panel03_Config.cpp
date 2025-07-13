// 2025/6/1 17:20:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config.h"
#include "MainWindow.h"


PanelConfig *PanelConfig::self = nullptr;


PanelConfig::PanelConfig(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1 + MainWindow::WIDTH2, 0, MainWindow::WIDTH3, HEIGHT)
{
    self = this;

    const int h = 20;

    wxSize sizeChan = { 67, h };

    new wxButton(this, ID_PAN3_BTN_CHANNEL_C, _L("Канал C"), { 0, 0 }, sizeChan);

    new wxButton(this, ID_PAN3_BTN_CHANNEL_B, _L("Канал B"), { sizeChan.x, 0 }, sizeChan);

    new wxButton(this, ID_PAN3_BTN_CHANNEL_S, _L("Канал S"), { sizeChan.x * 2, 0 }, sizeChan);

    wxSize sizeScheme = { 120, h };
    new wxButton(this, ID_PAN3_BTN_SHCEME, _L("Схема включения"), { 0, h }, sizeScheme);

    new wxButton(this, ID_PAN3_BTN_CALCULATION, _L("Расчёт"), { sizeScheme.x, h }, { MainWindow::WIDTH3 - sizeScheme.x - 3, h });

    /*
    wxAuiNotebook *notebook = new wxAuiNotebook(this, wxID_ANY);

    notebook->SetArtProvider(new wxAuiDefaultTabArt);
    notebook->SetWindowStyleFlag(wxAUI_NB_TAB_SPLIT | wxAUI_NB_SCROLL_BUTTONS);

    notebook->SetSize(MainWindow::WIDTH3, HEIGHT);

    notebook->AddPage(CreatePanel(notebook), "Канал C");
    notebook->AddPage(CreatePanel(notebook), "Канал B");
    notebook->AddPage(CreatePanel(notebook), "Канал S");
    notebook->AddPage(CreatePanel(notebook), "Схема включения");
    notebook->AddPage(CreatePanel(notebook), "Расчёт");

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(notebook, 1, wxEXPAND);
    SetSizer(sizer);

    SetSize(MainWindow::WIDTH3, HEIGHT);
    */
}


wxPanel *PanelConfig::CreatePanel(wxAuiNotebook *notebook)
{
    wxPanel *panel = new wxPanel(notebook);

    return panel;
}
