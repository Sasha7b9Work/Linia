// 2025/6/1 18:25:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "Panels/Panel07_Indicator.h"
#include "MainWindow.h"
#include "Utils/SystemDepend.h"


PanelIndicator *PanelIndicator::self = nullptr;


PanelIndicator::PanelIndicator(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1 + MainWindow::WIDTH2, PanelConfig::HEIGHT,
        MainWindow::WIDTH3, HEIGHT)
{
    self = this;

    new wxToggleButton(this, ID_BUTTON_MEASURE, "ИЗМЕРЕНИЕ", { 5, 10 }, { 90, BUTTON_HEIGHT });

    wxStaticText *text = new wxStaticText(this, wxID_ANY, "Uc 100 %", { 105, 5 }, { 25, 25});

    text->SetForegroundColour(wxColour(255, 0, 0));

    wxFont font = text->GetFont();
    font.SetPointSize(font.GetPointSize() + 7);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    text->SetFont(font);
}
