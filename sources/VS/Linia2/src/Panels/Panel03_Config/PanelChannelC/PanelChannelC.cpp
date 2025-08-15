// 2025/8/9 10:40:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/PanelChannelC/PanelChannelC.h"
#include "Utils/SystemDepend.h"
#include "Controls/SpinBox.h"
#include "MainWindow.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "Controls/CustomComboBox.h"
#include "Device/SettingsDevice.h"


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

    wxStaticBox *boxScan = new wxStaticBox(this, wxID_ANY, _L("Развёртка"), { x, 0 }, { w, 140 });          // IDC_STATICRAZV

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

        comboScan = new BmpButtonsCombo(boxScan, "Развёртка", { 18, SD::Y_SB(25) }, { 32, 42 }, files, tooltips, 0, 3);

        chbDutyCycle = new wxCheckBox(boxScan, wxID_ANY, _L("Скважн. x 2"), { 90, SD::Y_SB(35) }, { 100, 20 });

        wxArrayString names =
        {
            "20",
            "50"
        };

        comboNumberPoints = new ButtonsCombo(boxScan, "Число точек", { 10, SD::Y_SB(80) }, PanelConfig::WIDTH_COMBO, names, names, 1);

        textLabelImpulse = new wxStaticText(boxScan, wxID_ANY, "Длина импульса", { 10, SD::Y_SB(110) });                                    // IDC_STATICIMP
        textValueImpulse = new wxStaticText(boxScan, wxID_ANY, "0.2 ms",         { 120, SD::Y_SB(110) });                                   // IDC_EDITDLITIMP
    }

    wxStaticBox *boxMeter = new wxStaticBox(this, wxID_ANY, _L("Измеритель"), { x, boxScan->GetPosition().y + boxScan->GetSize().y }, { w, 50 });

    {
        int y = 20;

        wxArrayString names;

        RangeU::FillArrayStrings(names, DSet::Type::ChanC_Meas, false);
        comboVoltage = new ButtonsComboRange(boxMeter, "Uc", {10, SD::Y_SB(y)}, 80, names, names);

        RangeI::FillArrayStrings(names, DSet::Type::ChanC_Meas, false);
        comboCurrent = new ButtonsComboRange(boxMeter, "Ic", { 100, SD::Y_SB(y) }, 80, names, names);
    }

    wxStaticBox *boxSource = new wxStaticBox(this, wxID_ANY, "Источник U", { x, boxMeter->GetPosition().y + boxMeter->GetSize().y }, { w, h - boxMeter->GetPosition().y - boxMeter->GetSize().y });

    {
        wxArrayString names
        {
            "5 V",
            "20 V",
            "100 V",
            "500 V",
            "2 kV"
        };

        comboRange = new ButtonsCombo(boxSource, "Диапазон Ud", {10, SD::Y_SB(27)}, PanelConfig::WIDTH_COMBO, names, names, 3);

        new wxStaticText(boxSource, wxID_ANY, _L("Ограничение Uc, %%"), { 40, SD::Y_SB(60) });

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

        chbDutyCycle->Show(duty[scan]);

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
            comboNumberPoints->SetCurrentSelection(0);
        }
    }
}
