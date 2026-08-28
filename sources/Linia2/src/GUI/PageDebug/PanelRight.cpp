// 2026/01/24 21:42:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageDebug/PanelRight.h"
#include "MainWindow.h"
#include "Utils/SystemDepend.h"
#include "IPPP/IDevice.h"
#include "GUI/PageDebug/Notebook/PageFPGA.h"
#include "GUI/PageDebug/Notebook/PageChannelForm.h"
#include "Settings/Settings.h"
#include "GUI/Controls/TextControl.h"
#include "Utils/Math.h"
#include "Utils/Timer.h"
#pragma warning(push, 0)
#include <wx/sizer.h>
#pragma warning(pop)


PanelRight *ThePanelRight = nullptr;


PanelRight::PanelRight(wxWindow *parent, PanelRight *&global) :
    Panel(parent)
{
    global = this;

    wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);

    wxSize size_button{ 75, BUTTON_HEIGHT };

    {
        btnStart = new Button{ this, "Старт", size_button };

        btnStart->SetToolTip(L("Запуск развёртки"));

        txtPeriodScan = new TextCtrlNumber{ this, wxString::Format("%u", SET_DEBUG_PERIOD_SEND->GetUInt()), size_button,  10, 10000};

        txtPeriodScan->SetToolTip(L("Период запуска развёртки в миллисекундах"));

        wxBoxSizer *hor_sizer = new wxBoxSizer(wxHORIZONTAL);
        hor_sizer->Add(btnStart, 0, wxALL, 5);
        hor_sizer->Add(txtPeriodScan, 0, wxALL, 5);
        main_sizer->Add(hor_sizer, 0, wxEXPAND | wxTOP, 10);
    }

    {
        btnStop = new Button{ this, L("Стоп"), size_button };
        btnStop->SetToolTip(L("Останов развёртки"));
        btnStop->Enable(false);

        btnWriteData = new Button{ this, L("Сохранить"), size_button };
        btnWriteData->SetToolTip(L("Сохранить данные в файл /mnt/nvme/data"));

        Bind(wxEVT_BUTTON, &PanelRight::OnEventButton, this);

        wxBoxSizer *hor_sizer = new wxBoxSizer(wxHORIZONTAL);
        hor_sizer->Add(btnStop, 0, wxALL, 5);
        hor_sizer->Add(btnWriteData, 0, wxALL, 5);
        hor_sizer->AddStretchSpacer();
        main_sizer->Add(hor_sizer, 0, wxEXPAND | wxBOTTOM, 10);
    }

    {
        wxBoxSizer *ver_sizer = new wxBoxSizer(wxVERTICAL);

        for (int i = 0; i < 5; i++)
        {
            data[i] = new ControlDataFPGA(this);

            ver_sizer->Add(data[i], 1, wxEXPAND | wxALL, 5);
        }

        main_sizer->Add(ver_sizer, 1, wxEXPAND | wxALL, 0);
    }

    data[4]->SetMax((1 << 8) - 1);

    SetSizer(main_sizer);

    Layout();
}


void PanelRight::PeriodicTask()
{
    static TimeMeterMS timer;

    if (timer.ElapsedMS() < 1000)
    {
        return;
    }

    timer.Reset();

    static int MAX = ((1 << 18) - 1);

    int points[POINTS_IN_SAMPLE_ADC];

    for (int d = 0; d < 4; d++)
    {
        points[0] = Math::Rand(0, MAX);

        for (int i = 1; i < POINTS_IN_SAMPLE_ADC; i++)
        {
            int step = Math::Rand(-10000, 10000);

            int new_value = points[i - 1] + step;

            if (new_value < 0 || new_value > MAX)
            {
                new_value -= 2 * step;
            }

            points[i] = new_value;
        }

        data[d]->SetData(points);
    }
}


PanelRight::~PanelRight()
{
    SavePeriodScan();
}


void PanelRight::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnStart->GetId())
    {
        PageFPGA::self->SendAllRegisters();
        PageChannelForm::self->SendAllRegisters();

        IDevice::impl->SendCommand(wxString::Format(":SCAN:START %u", SavePeriodScan()));

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
    else if (id == btnWriteData->GetId())
    {
        need_write_data_to_file = true;
    }

    event.Skip();
}


uint PanelRight::SavePeriodScan()
{
    wxString str_value = txtPeriodScan->GetValue();
    uint uint_value = 0;
    str_value.ToUInt(&uint_value);

    SET_DEBUG_PERIOD_SEND->Set(uint_value);

    return uint_value;
}
