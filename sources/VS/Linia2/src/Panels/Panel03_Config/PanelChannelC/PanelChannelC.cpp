// 2025/8/9 10:40:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/PanelChannelC/PanelChannelC.h"
#include "Utils/SystemDepend.h"
#include "Controls/SpinBox.h"
#include "MainWindow.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "Controls/CustomComboBox.h"
#include "Controls/BmpButtonsCombo.h"


PanelChannelC::PanelChannelC(wxPanel *parent, int x, int w) :
    wxPanel(parent)
{
    SetSize({ MainWindow::WIDTH3, PanelConfig::HEIGHT - 40 });
    SetPosition({ 0, 40 });

    wxStaticBox *boxScan = new wxStaticBox(this, wxID_ANY, _L("Развёртка"), { x, 0 }, { w, 130 });

    {
        wxArrayString files =
        {
            "icons/graphs/imp_up.ico",      // IMP_POS  IDC_RADIO_RAZVIMPPOSITIVE   OnRadioRazvimppositive
            "icons/graphs/imp_down.ico",    // IMP_NEG  IDC_RADIO_RAZVIMPNEGATIVE
            "icons/graphs/triang_up.ico",   // DC_POS   IDC_RADIO_RAZVDCPOSITIVE
            "icons/graphs/triang_down.ico", // DC_NEG   IDC_RADIO_RAZVDCNEGATIVE
            "icons/graphs/sin_hi.ico",      // SYN_POS  IDC_RADIO_SYNPOSITIVE
            "icons/graphs/sin_lo.ico",      // SYN_NEG  IDC_RADIO_SYNNEGATIVE
            "icons/graphs/sin.ico"          // AC       IDC_RADIO_RAZVAC
        };

        wxArrayString tooltips =
        {
            "Положительные импульсы",
            "Отрицательные импульсы",
            "Положительное постоянное",
            "Отрицательное постоянное",
            "Положительные полуволны",
            "Отрицательные полуволны",
            "Синусоидальное"
        };

        new BmpButtonsCombo(boxScan, "Развёртка", { 18, SD::Y_SB(25) }, {32, 42}, files, tooltips, 0, 3);

        {
            wxStaticBox *boxImpulse = new wxStaticBox(boxScan, wxID_ANY, _L("Импульс"), { 100, SD::Y_SB(25) }, { 75, 40 });

            new wxStaticText(boxImpulse, wxID_ANY, "0.2 ms", { 10, SD::Y_SB(20) });
        }

        new wxCheckBox(boxScan, wxID_ANY, _L("Скважн. x 2"), { 20, SD::Y_SB(90) }, { 100, 20 });
    }

    wxStaticBox *boxMeter = new wxStaticBox(this, wxID_ANY, _L("Измеритель"), { x, boxScan->GetSize().y + x }, { w, 50 });

    {
        int y = 20;
        int dY = 3;

        new wxStaticText(boxMeter, wxID_ANY, "Uc", { 10, SD::Y_SB(y + dY) });
        new wxStaticText(boxMeter, wxID_ANY, "Ic", { 100, SD::Y_SB(y + dY) });

        wxArrayString choices;
        choices.Add("100V");

        new wxComboBox(boxMeter, wxID_ANY, choices[0], { 30, SD::Y_SB(y) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        choices.Clear();
        choices.Add("10A");

        new wxComboBox(boxMeter, wxID_ANY, choices[0], { 120, SD::Y_SB(y) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);
    }

    wxStaticBox *boxSource = new wxStaticBox(this, wxID_ANY, _L("Источник") + " U", { x, boxMeter->GetPosition().y + boxMeter->GetSize().y + x }, { w, 200 });

    {
        new wxStaticText(boxSource, wxID_ANY, _L("Диапазон Uc"), { 10, SD::Y_SB(30) });

        wxArrayString choices;
        choices.Add("20V");

        new wxComboBox(boxSource, wxID_ANY, choices[0], { 100, SD::Y_SB(27) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        new wxStaticText(boxSource, wxID_ANY, _L("Ограничение Uc, %%"), { 40, SD::Y_SB(55) });

        int y = 80;
        int dy = 3;

        new wxStaticText(boxSource, wxID_ANY, _L("Старт"), { 10, SD::Y_SB(y + dy) });
        new wxStaticText(boxSource, wxID_ANY, _L("Стоп"), { 10, SD::Y_SB(y + 30 + dy) });

        new wxSpinCtrl(boxSource, wxID_ANY, "0", { 80, SD::Y_SB(y) }, { 100, TEXTCNTRL_HEIGHT });
        new SpinBox(boxSource, wxID_ANY, "100", { 80, SD::Y_SB(y + 30) }, { 100, TEXTCNTRL_HEIGHT });

        CustomComboBox *combo = new CustomComboBox(boxSource, wxID_ANY);
        combo->SetPosition({ 10, 160 });
    }
}
