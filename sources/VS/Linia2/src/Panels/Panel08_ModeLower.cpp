// 2025/6/1 18:03:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel04_Model.h"
#include "Panels/Panel08_ModeLower.h"
#include "Utils/SystemDepend.h"
#include "Controls/StaticBox.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "Panels/Panel07_Indicator.h"


PanelModeLower *PanelModeLower::self = nullptr;


PanelModeLower::PanelModeLower(wxWindow *parent) :
    Panel(parent, MainWindow::WIDTH1 + MainWindow::WIDTH2, PanelConfig::HEIGHT + PanelIndicator::HEIGHT, WIDTH, HEIGHT)
{
    SetName("PanelModeLower");

    self = this;

    StaticBox *boxCategory = new StaticBox(this, "Режим", { 5, 0 }, { WIDTH - 13, HEIGHT - 8});

    {
        new wxStaticText(boxCategory, wxID_ANY, "Канал C:", { 10, SD::Y_SB(30) });

        new wxStaticText(boxCategory, wxID_ANY, "Канал B:", { 10, SD::Y_SB(60) });

        wxArrayString choices;
        choices.Add("C");

        new ButtonsCombo(boxCategory, "1 очередь", { 5, 120 }, 100, choices, choices, 1, "comboFirst");
    }

    boxCategory->SetFont(boxCategory->TitleFont());
}
