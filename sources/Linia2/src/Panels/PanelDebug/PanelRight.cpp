// 2026/01/24 21:42:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PanelRight.h"
#include "MainWindow.h"
#include "Utils/SystemDepend.h"
#include "IPPP/IDevice.h"
#include "Panels/PanelDebug/Notebook/PageFPGA.h"
#include "Panels/PanelDebug/Notebook/PageChannelC.h"


PanelRight *ThePanelRight = nullptr;


PanelRight::PanelRight(wxWindow *parent) :
    wxPanel(parent)
{
    ThePanelRight = this;

    wxSize size_button{ 75, BUTTON_HEIGHT };

    btnReturn = new wxButton(this, wxID_ANY, wxT("Закрыть"), { 125, SD::Y_SB(20) }, size_button);

    btnReturn->SetToolTip(wxT("Возврат в главную панель"));

    btnStart = new wxButton{ this, wxID_ANY, "Старт", { 10, SD::Y_SB(60) }, size_button };

    btnStart->SetToolTip(wxT("Запуск развёртки"));

    txtPeriodScan = new wxTextCtrl{ this, wxID_ANY, "1000", { 100, SD::Y_SB(60)}, size_button };

    txtPeriodScan->SetToolTip(wxT("Период запуска развёртки в миллисекундах"));

    btnStop = new wxButton{ this, wxID_ANY, "Стоп", {10, SD::Y_SB(90)}, size_button };

    btnStop->SetToolTip(wxT("Останов развёртки"));

    btnStop->Enable(false);

    Bind(wxEVT_BUTTON, &PanelRight::OnEventButton, this);

    for (int i = 0; i < 5; i++)
    {
        data[i] = new ControlDataFPGA(this, { 10, 130 + i * 95 });
    }

    data[4]->SetMax((1 << 8) - 1);

    Fit();
    Layout();
}


void PanelRight::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnReturn->GetId())
    {
        TheMainWindow->SetMode(ModeMainWindow::Standard);
    }
    else if (id == btnStart->GetId())
    {
        wxString str_value = txtPeriodScan->GetValue();
        int int_value = 0;
        str_value.ToInt(&int_value);

        PageFPGA::self->SendAllRegisters();
        PageChannelC::self->SendAllRegisters();

        IDevice::impl->SendCommand(wxString::Format(":SCAN:START %d", int_value));

        btnStart->Enable(false);
        btnStop->Enable(true);
        txtPeriodScan->Enable(false);
    }
    else if (id == btnStop->GetId())
    {
        IDevice::impl->SendCommand(":SCAN:STOP");

        btnStart->Enable(true);
        btnStop->Enable(false);
        txtPeriodScan->Enable(true);
    }
}
