// 2025/08/14 20:09:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/PanelCalculate.h"
#include "MainWindow.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "Utils/SystemDepend.h"
#include "Controls/StaticBox.h"


PanelCalculate *PanelCalculate::self = nullptr;


PanelCalculate::PanelCalculate(wxPanel *parent, int x, int w, int h) :
    wxPanel(parent)
{
    SetName("PanelCalculate");

    self = this;

    SetSize({ MainWindow::WIDTH3, PanelConfig::HEIGHT - 40 });
    SetPosition({ 0, 40 });

    StaticBox *boxGraph = new StaticBox(this, _L("График"), { x, SD::DSBY() }, { w, 75 });

    int y = 20;
    const int dy = 3;

    {
        new wxStaticText(boxGraph, wxID_ANY, _L("Ось X"), { PanelConfig::X, SD::Y_SB(y + dy) });

        new wxTextCtrl(boxGraph, wxID_ANY, "Ud", { 100, SD::Y_SB(y) }, { 50, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxGraph, wxID_ANY, _L("Ось Y"), { PanelConfig::X, SD::Y_SB(y + dy) });

        new wxTextCtrl(boxGraph, wxID_ANY, "Id", { 100, SD::Y_SB(y) }, { 50, TEXTCNTRL_HEIGHT });
    }

    boxGraph->SetFont(StaticBox::TitleFont());

    StaticBox *boxParameter = new StaticBox(this, _L("Параметр"),
        { x, boxGraph->GetPosition().y + boxGraph->GetSize().y + SD::DSBY() },
        { w, 250 });

    {
        y = 20;

        new wxStaticText(boxParameter, wxID_ANY, _L("Найти"), { PanelConfig::X, SD::Y_SB(y + dy) });
        new wxStaticText(boxParameter, wxID_ANY, _L("при"), { 100, SD::Y_SB(y + dy) });

        new wxTextCtrl(boxParameter, wxID_ANY, "Id", { 50, SD::Y_SB(y) }, { 40, TEXTCNTRL_HEIGHT });
        new wxTextCtrl(boxParameter, wxID_ANY, "Ud", { 140, SD::Y_SB(y) }, { 40, TEXTCNTRL_HEIGHT });

        y += 30;

        new wxStaticText(boxParameter, wxID_ANY, _L("равном"), { PanelConfig::X, SD::Y_SB(y) });

        y += 20;

        new wxStaticText(boxParameter, wxID_ANY, _L("значению") + " 1", { PanelConfig::X, SD::Y_SB(y + dy) });
        new wxTextCtrl(boxParameter, wxID_ANY, "6", { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxParameter, wxID_ANY, _L("значению") + " 2", { PanelConfig::X, SD::Y_SB(y + dy) });
        new wxTextCtrl(boxParameter, wxID_ANY, "6", { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxParameter, wxID_ANY, _L("на ступеньке") + " № 5", { PanelConfig::X, SD::Y_SB(y + dy) });

        StaticBox *boxCalculate = new StaticBox(boxParameter, _L("Рассчитать"), { x, y }, { w - 10, 60 });

        {
            y = 25;

            new wxRadioButton(boxCalculate, wxID_ANY, "dY/dX", { PanelConfig::X, SD::Y_SB(y) });
            new wxRadioButton(boxCalculate, wxID_ANY, "dX/dY", { 100, SD::Y_SB(y) });
        }

        boxCalculate->SetFont(StaticBox::TitleFont());

        wxPoint pos = boxCalculate->GetPosition();
        pos.y = SD::Y_SB(boxParameter->GetSize().y - boxCalculate->GetSize().y - 8);
        boxCalculate->SetPosition(pos);
    }

    boxParameter->SetFont(StaticBox::TitleFont());

    StaticBox *boxBorder = new StaticBox(this, _L("Пороговые значения"),
        { x, boxParameter->GetPosition().y + boxParameter->GetSize().y + SD::DSBY() },
        { w, h - boxParameter->GetPosition().y - boxParameter->GetSize().y - SD::DSBY() });

    {
        new wxStaticText(boxBorder, wxID_ANY, "MIN", { PanelConfig::X, SD::Y_SB(y + dy) });
        new wxTextCtrl(boxBorder, wxID_ANY, "3", { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxBorder, wxID_ANY, "MAX", { PanelConfig::X, SD::Y_SB(y + dy) });
        new wxTextCtrl(boxBorder, wxID_ANY, "4", { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT });
    }

    boxBorder->SetFont(StaticBox::TitleFont());
}
