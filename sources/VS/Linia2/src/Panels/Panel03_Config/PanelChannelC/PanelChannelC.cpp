// 2025/8/9 10:40:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/PanelChannelC/PanelChannelC.h"
#include "Utils/SystemDepend.h"
#include "Controls/SpinBox.h"
#include "MainWindow.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "Controls/CustomComboBox.h"
#include "Device/SettingsDevice.h"
#include "Controls/StaticBox.h"


/*
    Не найдено
    IDC_STATICIMP1POS
    IDC_STATICIMP1NEG
    IDC_CHECKPAUZA
*/


PanelChannelC *PanelChannelC::self = nullptr;


PanelChannelC::PanelChannelC(wxPanel *parent, int x, int w, int h) :
    wxPanel(parent)
{
    self = this;

    SetSize({ MainWindow::WIDTH3, PanelConfig::HEIGHT - 40 });
    SetPosition({ 0, 40 });

    StaticBox *boxScan = new StaticBox(this, _L("Развёртка"), { x, SD::DSBY() }, { w, 140 });          // IDC_STATICRAZV

    {
        wxArrayString files =
        {
            "icons/graphs/imp_up.ico",      // IMP_POS  IDC_RADIO_RAZVIMPPOSITIVE   IDC_STATICIMPPOSITIVE   OnRadioRazvimppositive
            "icons/graphs/imp_down.ico",    // IMP_NEG  IDC_RADIO_RAZVIMPNEGATIVE   IDC_STATICIMPNEGATIVE
            "icons/graphs/triang_up.ico",   // DC_POS   IDC_RADIO_RAZVDCPOSITIVE
            "icons/graphs/triang_down.ico", // DC_NEG   IDC_RADIO_RAZVDCNEGATIVE
            "icons/graphs/sin_hi.ico",      // SYN_POS  IDC_RADIO_SYNPOSITIVE
            "icons/graphs/sin_lo.ico",      // SYN_NEG  IDC_RADIO_SYNNEGATIVE
            "icons/graphs/sin.ico"          // AC       IDC_RADIO_RAZVAC            IDC_STATICAC
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

        comboScan = new BmpButtonsCombo(boxScan, "Развёртка", { 18, SD::XY0().y }, { 32, 42 }, files, tooltips, 0, 3, "ChannelC_Scan");

        int y = 25;
        int x0 = 80;

        textLabelImpulse = new wxStaticText(boxScan, wxID_ANY, "Длина импульса", { x0, SD::Y_SB(y) });                  // IDC_STATICIMP
        textValueImpulse = new wxStaticText(boxScan, wxID_ANY, "0.2 ms", { x0 + 30, SD::Y_SB(y + 15) });                // IDC_EDITDLITIMP

        y = 80;

        wxArrayString names = { "20", "50" };

        comboNumberPoints = new ButtonsCombo(boxScan, "Число точек", { PanelConfig::X, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO, names, names, 1);

        y += PanelConfig::DYC;

        names = { "x 1", "x 2" };

        comboDutyCycleIncrease = new ButtonsCombo(boxScan, "Скважность", { PanelConfig::X, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO, names, names, 1);
    }

    boxScan->SetFont(StaticBox::TitleFont());

    StaticBox *boxMeter = new StaticBox(this, _L("Измеритель"), { x, boxScan->GetPosition().y + boxScan->GetSize().y + SD::DSBY() }, { w, 50 });

    {
        wxArrayString names;
        int width = 85;

        RangeU::FillArrayStrings(names, DSet::Type::ChanC_Meas, false);
        comboVoltage = new ButtonsComboRange(boxMeter, "Uc", SD::XY0(), width, names, names);

        RangeI::FillArrayStrings(names, DSet::Type::ChanC_Meas, false);
        comboCurrent = new ButtonsComboRange(boxMeter, "Ic", { SD::XY0().x + PanelConfig::WIDTH_COMBO - width, SD::XY0().y }, width, names, names);
    }

    boxMeter->SetFont(StaticBox::TitleFont());

    StaticBox *boxSource = new StaticBox(this, "Источник U", { x, boxMeter->GetPosition().y + boxMeter->GetSize().y + SD::DSBY() }, { w, h - boxMeter->GetPosition().y - boxMeter->GetSize().y - SD::DSBY() });

    {
        wxArrayString names
        {
            "5 V",
            "20 V",
            "100 V",
            "500 V",
            "2 kV"
        };

        comboRange = new ButtonsCombo(boxSource, "Диапазон Ud", SD::XY0(), PanelConfig::WIDTH_COMBO, names, names, 1);

        new wxStaticText(boxSource, wxID_ANY, _L("Ограничение Uc, %%"), { 40, SD::Y_SB(65) });

        int y = 90;

        new wxStaticText(boxSource, wxID_ANY, _L("Старт"), { 10, SD::Y_SB(y) });

        y += 20;

        int width = 180;
        spinStart = new SliderInt(boxSource, { 10, SD::Y_SB(y) }, width, 0, 100);

        y += 40;

        new wxStaticText(boxSource, wxID_ANY, _L("Стоп"), { 10, SD::Y_SB(y) });

        y += 20;

        spinStart = new SliderInt(boxSource, { 10, SD::Y_SB(y) }, width, 0, 100);
    }

    boxSource->SetFont(StaticBox::TitleFont());

    Bind(wxEVT_COMBOBOX, &PanelChannelC::OnEventComboBox, this);

    Tune();
}


void PanelChannelC::Tune()
{
    comboScan->SetCurrentChoice(1);
}


void PanelChannelC::OnEventComboBox(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == comboRange->GetId())
    {
        static const pchar values[5] =
        {
            "0.2 ms",
            "0.2 ms",
            "0.5 ms",
            "1.0 ms",
            "2.0 ms"
        };

        textValueImpulse->SetLabel(values[comboRange->GetCurrentSelection()]);
    }
    else if (id == comboScan->GetId())
    {
        int scan = comboScan->GetCurrentChoice();

        bool show = scan < 2;

        textLabelImpulse->Show(show);
        textValueImpulse->Show(show);

        static const bool duty[7] =
        {
            true, true, false, false, true, true, true
        };

        comboDutyCycleIncrease->Show(duty[scan]);

        {
            wxArrayString choices;

            if (scan < 4)
            {
                choices.push_back("20");
                choices.push_back("50");
            }
            else
            {
                choices.push_back("200");
            }

            comboNumberPoints->SetChoices(choices, choices);
        }
    }
}
