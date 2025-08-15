// 2025/08/14 20:09:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/PanelCalculate.h"
#include "MainWindow.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "Utils/SystemDepend.h"


PanelCalculate *PanelCalculate::self = nullptr;


PanelCalculate::PanelCalculate(wxPanel *parent, int x, int w, int h) :
    wxPanel(parent)
{
    self = this;

    SetSize({ MainWindow::WIDTH3, PanelConfig::HEIGHT - 40 });
    SetPosition({ 0, 40 });

    wxStaticBox *boxGraph = new wxStaticBox(this, wxID_ANY, _L("График"), { x, 0 }, { w, 75 });

    int y = 20;
    const int dy = 3;

    {
        new wxStaticText(boxGraph, wxID_ANY, _L("Ось X"), { 10, SD::Y_SB(y + dy) });

        new wxTextCtrl(boxGraph, wxID_ANY, "Ud", { 100, SD::Y_SB(y) }, { 50, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxGraph, wxID_ANY, _L("Ось Y"), { 10, SD::Y_SB(y + dy) });

        new wxTextCtrl(boxGraph, wxID_ANY, "Id", { 100, SD::Y_SB(y) }, { 50, TEXTCNTRL_HEIGHT });
    }

    wxStaticBox *boxParameter = new wxStaticBox(this, wxID_ANY, _L("Параметр"),
        { x, boxGraph->GetPosition().y + boxGraph->GetSize().y },
        { w, 250 });

    {
        y = 20;

        new wxStaticText(boxParameter, wxID_ANY, _L("Найти"), { 10, SD::Y_SB(y + dy) });
        new wxStaticText(boxParameter, wxID_ANY, _L("при"), { 100, SD::Y_SB(y + dy) });

        new wxTextCtrl(boxParameter, wxID_ANY, "Id", { 50, SD::Y_SB(y) }, { 40, TEXTCNTRL_HEIGHT });
        new wxTextCtrl(boxParameter, wxID_ANY, "Ud", { 140, SD::Y_SB(y) }, { 40, TEXTCNTRL_HEIGHT });

        y += 30;

        new wxStaticText(boxParameter, wxID_ANY, _L("равном"), { 10, SD::Y_SB(y) });

        y += 20;

        new wxStaticText(boxParameter, wxID_ANY, _L("значению") + " 1", { 10, SD::Y_SB(y + dy) });
        new wxTextCtrl(boxParameter, wxID_ANY, "6", { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxParameter, wxID_ANY, _L("значению") + " 2", { 10, SD::Y_SB(y + dy) });
        new wxTextCtrl(boxParameter, wxID_ANY, "6", { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxParameter, wxID_ANY, _L("на ступеньке") + " № 5", { 10, SD::Y_SB(y + dy) });

        wxStaticBox *boxCalculate = new wxStaticBox(boxParameter, wxID_ANY, _L("Рассчитать"), { x, y }, { w - 10, 60 });

        {
            y = 25;

            new wxRadioButton(boxCalculate, wxID_ANY, "dY/dX", { 10, SD::Y_SB(y) });
            new wxRadioButton(boxCalculate, wxID_ANY, "dX/dY", { 100, SD::Y_SB(y) });
        }

        wxPoint pos = boxCalculate->GetPosition();
        pos.y = SD::Y_SB(boxParameter->GetSize().y - boxCalculate->GetSize().y - 8);
        boxCalculate->SetPosition(pos);
    }

    wxStaticBox *boxBorder = new wxStaticBox(this, wxID_ANY, _L("Пороговые значения"),
        { x, boxParameter->GetPosition().y + boxParameter->GetSize().y },
        { w, h - boxParameter->GetPosition().y - boxParameter->GetSize().y });

    {
        new wxStaticText(boxBorder, wxID_ANY, "MIN", { 10, SD::Y_SB(y + dy) });
        new wxTextCtrl(boxBorder, wxID_ANY, "3", { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxBorder, wxID_ANY, "MAX", { 10, SD::Y_SB(y + dy) });
        new wxTextCtrl(boxBorder, wxID_ANY, "4", { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT });
    }
}
