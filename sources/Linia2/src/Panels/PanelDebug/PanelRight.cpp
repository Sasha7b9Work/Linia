// 2026/01/24 21:42:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PanelRight.h"
#include "MainWindow.h"
#include "Utils/SystemDepend.h"
#include "IPPP/Device/IDevice.h"


PanelRight *PanelRight::self = nullptr;


PanelRight::PanelRight(wxWindow *parent) : wxPanel(parent)
{
    self = this;

    wxSize size_button{ 75, BUTTON_HEIGHT };

    btnReturn = new wxButton(this, wxID_ANY, "Закрыть", { 125, SD::Y_SB(20) }, size_button);

    btnReturn->SetToolTip("Возврат в главную панель");

    btnStart = new wxButton{ this, wxID_ANY, "Старт", { 10, SD::Y_SB(60) }, size_button };

    btnStart->SetToolTip("Запуск развёртки");

    txtPeriodScan = new wxTextCtrl{ this, wxID_ANY, "1000", { 100, SD::Y_SB(60)}, size_button };

    txtPeriodScan->SetToolTip("Период запуска развёртки в миллисекундах");

    btnStop = new wxButton{ this, wxID_ANY, "Стоп", {10, SD::Y_SB(90)}, size_button };

    btnStop->SetToolTip("Останов развёртки");

    btnStop->Enable(false);

    Bind(wxEVT_BUTTON, &PanelRight::OnEventButton, this);
}


void PanelRight::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnReturn->GetId())
    {
        MainWindow::self->SetMode(ModeMainWindow::Standard);
    }
    else if (id == btnStart->GetId())
    {
        wxString str_value = txtPeriodScan->GetValue();
        int int_value = 0;
        str_value.ToInt(&int_value);

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
