// 2025/09/03 09:54:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel12_Debug/PageTestsGPIO.h"
#include "Utils/SystemDepend.h"


PageTestsGPIO *PageTestsGPIO::self = nullptr;

bool PageTestsGPIO::thread_is_running = false;


PageTestsGPIO::PageTestsGPIO(wxNotebook *parent) :
    wxPanel(parent)
{
    self = this;

    wxPanel::SetName("Тесты разъёма GPIO");

    wxStaticBox *boxGPIO = new wxStaticBox(this, wxID_ANY, "GPIO", { 10, 10 }, { 200, 270 });

    {
        int x = 10;
        int y = 20;
        int dy = 31;


        static PinIn *pins_in[] =
        {
            &pinSTART,
            &pinSTOP,
            &pinDAT_F0,
            &pinDAT_F1,
            &pinDAT_F2,
            &pinDAT_F3,
            &pinFIFO_FULL,
            nullptr
        };

        pinSTART.SetChangeCallback(CallbackOnStart);

        int index = 0;

        PinIn *in = pins_in[index];

        while (in)
        {
            wxPanel *panel = CreatePanelPinIn(boxGPIO, in);

            panel->SetPosition({ x, SD::Y_SB(y) });

            y += dy;

            in = pins_in[++index];
        }

        static PinOut *pins_out[] =
        {
            &pinREQ_RD,
            nullptr
        };

        index = 0;

        PinOut *out = pins_out[index];

        while (out)
        {
            wxPanel *panel = CreatePanelPinOut(boxGPIO, out);

            panel->SetPosition({ x, SD::Y_SB(y) });

            y += dy;

            out = pins_out[++index];
        }
    }

    wxStaticBox *boxUART = new wxStaticBox(this, wxID_ANY, "UART", { boxGPIO->GetPosition().x + boxGPIO->GetSize().x + 10, 10 }, { 200, 270 });

    {
        new wxStaticText(boxUART, wxID_ANY, "TX : 8", { 10, 20 });
        new wxStaticText(boxUART, wxID_ANY, "RX : 10", { 10, 45 });

        new wxTextCtrl(boxUART, wxID_ANY, "", { 10, 70 }, { 100, 20 });
        btnSendUART = new wxButton(boxUART, wxID_ANY, "Send", { 120, 70 }, { 50, 20 });

        new wxStaticText(boxUART, wxID_ANY, "Принято:", { 10, 105 });
        new wxTextCtrl(boxUART, wxID_ANY, "", { 10, 130 }, { 100, 20 }, wxTE_READONLY);
    }

    wxStaticBox *boxSPI = new wxStaticBox(this, wxID_ANY, "SPI", { boxUART->GetPosition().x + boxUART->GetSize().x + 10, 10 }, { 200, 270 });

    {
        new wxStaticText(boxSPI, wxID_ANY, "MOSI : 19", { 10, 20 });
        new wxStaticText(boxSPI, wxID_ANY, "CLK : 23", { 10, 45 });

        new wxTextCtrl(boxSPI, wxID_ANY, "", { 10, 70 }, { 100, 20 });
        btnSendSPI = new wxButton(boxSPI, wxID_ANY, "Send", { 120, 70 }, { 50, 20 });
    }

    Bind(wxEVT_BUTTON, &PageTestsGPIO::OnEventButton, this);
}


wxString PageTestsGPIO::NamePin(Pin::Type pin) const
{
    static const pchar names[Pin::Count] =
    {
        "START",
        "STOP",
        "DAT_F0",
        "DAT_F1",
        "DAT_F2",
        "DAT_F3",
        "FIFO_FULL",
        "REQ_RD"
    };

    return names[pin];
}


int PageTestsGPIO::NumPin(Pin::Type pin) const
{
    static const int num[Pin::Count] =
    {
        15,
        21,
        16,
        18,
        22,
        24,
        36,
        32
    };

    return num[pin];
}


wxPanel *PageTestsGPIO::CreatePanelPinOut(wxWindow *parent, PinOut *pin)
{
    wxPanel *panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, { 180, 23 });

    StructOutGPIO strGPIO(pin);

    strGPIO.button = new wxButton(panel, wxID_ANY, NamePin(pin->type()), { 0, 0 }, { 70, 22 });

    strGPIO.button->Bind(wxEVT_BUTTON, &PageTestsGPIO::OnEventButton, this);

    strGPIO.txtStatePull = new wxTextCtrl(panel, wxID_ANY, "", { 100, 0 }, { 20, 22 }, wxTE_READONLY);

    strGPIO.txtStateInt = new wxTextCtrl(panel, wxID_ANY, "", { 130, 0 }, { 20, 22 }, wxTE_READONLY);

    new wxStaticText(panel, wxID_ANY, wxString::Format("%d", NumPin(pin->type())), { 160, 2 }, { 40, 22 });

    gpio_out.push_back(strGPIO);

    return panel;
}


wxPanel *PageTestsGPIO::CreatePanelPinIn(wxWindow *parent, PinIn *pin)
{
    wxPanel *panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, { 180, 23 });

    StructInGPIO strGPIO(pin);

    new wxStaticText(panel, wxID_ANY, NamePin(pin->type()), { 0, 2 }, { 80, 22 });

    strGPIO.txtStatePull = new wxTextCtrl(panel, wxID_ANY, "", { 100, 0 }, { 20, 22 }, wxTE_READONLY);

    strGPIO.txtStateInt = new wxTextCtrl(panel, wxID_ANY, "", { 130, 0 }, { 20, 22 }, wxTE_READONLY);

    new wxStaticText(panel, wxID_ANY, wxString::Format("%d", NumPin(pin->type())), { 160, 2 }, { 40, 22 });

    gpio_in.push_back(strGPIO);

    return panel;
}


void PageTestsGPIO::OnEventButton(wxCommandEvent &event)
{
    event.Skip();

    int id = event.GetId();

    if (id == btnSendSPI->GetId())
    {

    }
    else if (id == btnSendUART->GetId())
    {

    }
    else
    {
        for (auto &pin : gpio_out)
        {
            if (id == pin.button->GetId())
            {
                return;
            }
        }
    }
}


void PageTestsGPIO::ThreadFunc()
{
    while (thread_is_running)
    {
        for (auto &str : PageTestsGPIO::self->gpio_out)
        {
            str.txtStatePull->SetValue(str.pin->Get() ? "1" : "0");
        }

        for (auto &str : PageTestsGPIO::self->gpio_in)
        {
            str.txtStatePull->SetValue(str.pin->Get() ? "1" : "0");
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}


void PageTestsGPIO::OnChangeStatePin(PinIn *pin, bool state)
{
    for (auto &str : PageTestsGPIO::self->gpio_in)
    {
        if (str.pin == pin)
        {
            str.txtStateInt->SetValue(state ? "1" : "0");
        }
    }
}


void PageTestsGPIO::OnChangeStatePin(PinOut *pin, bool state)
{
    for (auto &str : PageTestsGPIO::self->gpio_out)
    {
        if (str.pin == pin)
        {
            str.txtStateInt->SetValue(state ? "1" : "0");
        }
    }
}


void PageTestsGPIO::Init()
{
    if (thread)
    {
        return;
    }

    thread_is_running = true;

    thread = new std::thread(ThreadFunc);

    thread->detach();
}


void PageTestsGPIO::DeInit()
{
    if(!thread)
    {
        return;
    }

    thread_is_running = false;

    while (thread->joinable())
    {
    }

    SAFE_DELETE(thread);
}


void PageTestsGPIO::CallbackOnStart(bool state)
{
    PageTestsGPIO::self->OnChangeStatePin(&pinSTART, state);
}

void PageTestsGPIO::CallbackOnStop(bool state)
{
    PageTestsGPIO::self->OnChangeStatePin(&pinSTOP, state);
}

void PageTestsGPIO::CallbackOnDAT_F0(bool state)
{
    PageTestsGPIO::self->OnChangeStatePin(&pinDAT_F0, state);
}

void PageTestsGPIO::CallbackOnDAT_F1(bool state)
{
    PageTestsGPIO::self->OnChangeStatePin(&pinDAT_F1, state);
}

void PageTestsGPIO::CallbackOnDAT_F2(bool state)
{
    PageTestsGPIO::self->OnChangeStatePin(&pinDAT_F2, state);
}

void PageTestsGPIO::CallbackOnDAT_F3(bool state)
{
    PageTestsGPIO::self->OnChangeStatePin(&pinDAT_F3, state);
}

void PageTestsGPIO::CallbackOnFIFO_FULL(bool state)
{
    PageTestsGPIO::self->OnChangeStatePin(&pinFIFO_FULL, state);
}

void PageTestsGPIO::CallbackonREQ_RD(bool state)
{
    PageTestsGPIO::self->OnChangeStatePin(&pinREQ_RD, state);
}
