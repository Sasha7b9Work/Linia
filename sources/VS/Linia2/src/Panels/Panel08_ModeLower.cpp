// 2025/6/1 18:03:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel04_Model.h"
#include "Panels/Panel06_Percents.h"
#include "Panels/Panel08_ModeLower.h"
#include "Utils/SystemDepend.h"


PanelModeLower *PanelModeLower::self = nullptr;


PanelModeLower::PanelModeLower(wxWindow *parent) :
    Panel(parent, 0, MainWindow::HEIGTH1 + PanelModel::HEIGHT + PanelPercents::HEIGHT, WIDTH, HEIGHT)
{
    self = this;

    wxStaticBox *boxCategory = new wxStaticBox(this, wxID_ANY, _L("Режим"), { 5, 0 }, { WIDTH - 13, HEIGHT - 8});

    {
        new wxStaticText(boxCategory, wxID_ANY, _L("Канал") + " C:", { 10, SD::Y_SB(30) });

        new wxStaticText(boxCategory, wxID_ANY, _L("Канал") + " B:", { 10, SD::Y_SB(60) });

        new wxStaticText(boxCategory, wxID_ANY, wxString("1 ") + _L("очередь"), { 5, SD::Y_SB(100) });

        wxArrayString choices;
        choices.Add(_L("Канал") + " C");

        new wxComboBox(boxCategory, ID_PAN8_COMBO, choices[0], { 5, 120 }, { 80, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);
    }
}
