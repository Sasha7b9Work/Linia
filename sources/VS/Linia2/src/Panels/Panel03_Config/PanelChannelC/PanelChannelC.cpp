// 2025/8/9 10:40:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/PanelChannelC/PanelChannelC.h"
#include "Utils/SystemDepend.h"
#include "Controls/SpinBox.h"
#include "MainWindow.h"
#include "Panels/Panel03_Config/Panel03_Config.h"


PanelChannelC::PanelChannelC(wxPanel *parent, int x, int w) :
    wxPanel(parent)
{
    SetSize({ MainWindow::WIDTH3, PanelConfig::HEIGHT - 40 });
    SetPosition({ 0, 40 });

    Bind(wxEVT_RADIOBUTTON, &PanelChannelC::OnEventRadioButton, this);

    wxStaticBox *boxScan = new wxStaticBox(this, wxID_ANY, _L("Развёртка"), { x, 0 }, { w, 160 });

    {
        const wxSize size_rb{ 50, 30 };

        rbScan[0] = new RadioButtonICO(boxScan, { 10, SD::Y_SB(20) }, size_rb, "icons/graphs/imp_up.ico");      // IMP_POS  IDC_RADIO_RAZVIMPPOSITIVE   OnRadioRazvimppositive
        rbScan[1] = new RadioButtonICO(boxScan, { 70, SD::Y_SB(20) }, size_rb, "icons/graphs/imp_down.ico");    // IMP_NEG  IDC_RADIO_RAZVIMPNEGATIVE

        rbScan[2] = new RadioButtonICO(boxScan, { 10, SD::Y_SB(50) }, size_rb, "icons/graphs/triang_up.ico");   // DC_POS   IDC_RADIO_RAZVDCPOSITIVE
        rbScan[3] = new RadioButtonICO(boxScan, { 70, SD::Y_SB(50) }, size_rb, "icons/graphs/triang_down.ico"); // DC_NEG   IDC_RADIO_RAZVDCNEGATIVE

        rbScan[4] = new RadioButtonICO(boxScan, { 10, SD::Y_SB(80) }, size_rb, "icons/graphs/sin_hi.ico");      // SYN_POS  IDC_RADIO_SYNPOSITIVE
        rbScan[5] = new RadioButtonICO(boxScan, { 70, SD::Y_SB(80) }, size_rb, "icons/graphs/sin_lo.ico");      // SYN_NEG  IDC_RADIO_SYNNEGATIVE

        rbScan[6] = new RadioButtonICO(boxScan, { 130, SD::Y_SB(20) }, size_rb, "icons/graphs/sin.ico");        // AC       IDC_RADIO_RAZVAC

        {
            RadioButtonICO *object = rbScan[0];
            wxCommandEvent evt(wxEVT_RADIOBUTTON, object->GetId());
            evt.SetInt(1);
            evt.SetEventObject(object);
            boxScan->ProcessWindowEvent(evt);
        }

        {
            wxStaticBox *boxImpulse = new wxStaticBox(boxScan, wxID_ANY, _L("Импульс"), { 10, SD::Y_SB(110) }, { 75, 40 });

            new wxStaticText(boxImpulse, wxID_ANY, "0.2 ms", { 10, SD::Y_SB(20) });
        }

        new wxCheckBox(boxScan, wxID_ANY, _L("Скважн. x 2"), { 100, SD::Y_SB(120) }, { 100, 20 });
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
    }
}


void PanelChannelC::OnEventRadioButton(wxCommandEvent &event)
{
    RadioButtonICO *object = ((RadioButtonICO *)event.GetEventObject());

    for (int i = 0; i < 7; i++)
    {
        bool value = (object == rbScan[i]);
        rbScan[i]->SetValue(value);
    }

    if (object == rbScan[0])        // IDC_RADIO_RAZVIMPPOSITIVE OnRadioRazvimppositive()
    {
        /*
        int irazv_old = m_irazv;
        if (n_IPPP == 1 && n_Switch == 2)
        {
            m_irazv = IMP_NEG;
        }
        InitIconImp();
        ChangeDiapazonKol();
        ChangeDiapazonKolMeasU();
        ShowRejimChanel();
        ShowButtonLoopingCompensation();
        if (iX == COLLECTOR && iY == COLLECTOR) ResetDisplay(LEFT);
        int er = ControlParam();
        */
    }
}
