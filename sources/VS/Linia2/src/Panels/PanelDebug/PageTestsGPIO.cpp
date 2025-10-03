// 2025/09/03 09:54:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PageTestsGPIO.h"
#include "Utils/SystemDepend.h"
#include "Communicator/UART/UART.h"


PageTestsGPIO *PageTestsGPIO::self = nullptr;

bool PageTestsGPIO::thread_is_running = false;
bool PageTestsGPIO::thread_autoUART_is_running = false;


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

    wxStaticBox *boxFPGA = new wxStaticBox(this, wxID_ANY, "FPGA", { boxGPIO->GetPosition().x,
        boxGPIO->GetPosition().y + boxGPIO->GetSize().y + 10 }, { 400, 270 });

    {
        new wxStaticText(boxFPGA, wxID_ANY, "Кол-во измерений", { 10, 20 });
        txtNumberMeas = new wxTextCtrl(boxFPGA, wxID_ANY, "0", { 150, 20 });

        new wxStaticText(boxFPGA, wxID_ANY, "Кол-во FULL", { 10, 50 });
        txtNumberFULL = new wxTextCtrl(boxFPGA, wxID_ANY, "0", { 150, 50 });
    }

    wxStaticBox *boxUART = new wxStaticBox(this, wxID_ANY, "UART", { boxGPIO->GetPosition().x + boxGPIO->GetSize().x + 10, 10 }, { 200, 270 });

    {
        new wxStaticText(boxUART, wxID_ANY, "TX : 8", { 10, 20 });
        new wxStaticText(boxUART, wxID_ANY, "RX : 10", { 10, 45 });

        new wxTextCtrl(boxUART, wxID_ANY, "", { 10, 70 }, { 100, 20 });
        btnSendUART = new wxButton(boxUART, wxID_ANY, "Send", { 120, 70 }, { 50, 20 });

        new wxStaticText(boxUART, wxID_ANY, "Принято:", { 10, 105 });
        new wxTextCtrl(boxUART, wxID_ANY, "", { 10, 130 }, { 100, 20 }, wxTE_READONLY);

        btnAutoUART = new wxToggleButton(boxUART, wxID_ANY, "AutoSend", { 10, 170 }, { 100, 20 });
    }

    wxStaticBox *boxSPI = new wxStaticBox(this, wxID_ANY, "SPI", { boxUART->GetPosition().x + boxUART->GetSize().x + 10, 10 }, { 200, 270 });

    {
        new wxStaticText(boxSPI, wxID_ANY, "MOSI : 19", { 10, 20 });
        new wxStaticText(boxSPI, wxID_ANY, "CLK : 23", { 10, 45 });

        new wxTextCtrl(boxSPI, wxID_ANY, "", { 10, 70 }, { 100, 20 });
        btnSendSPI = new wxButton(boxSPI, wxID_ANY, "Send", { 120, 70 }, { 50, 20 });
    }

    wxStaticBox *boxEncoder = new wxStaticBox(this, wxID_ANY, "Encoder", { boxSPI->GetPosition().x + boxSPI->GetSize().x + 10, 10 }, { 200, 270 });

    {
        new wxStaticText(boxEncoder, wxID_ANY, "KA : 11", { 10, 20 });
        txtKA = new wxTextCtrl(boxEncoder, wxID_ANY, "0", { 50, 20 }, { 30, 20 }, wxTE_READONLY);

        new wxStaticText(boxEncoder, wxID_ANY, "KB : 13", { 10, 45 });
        txtKB = new wxTextCtrl(boxEncoder, wxID_ANY, "0", { 50, 45 }, { 30, 20 }, wxTE_READONLY);
    }

    Bind(wxEVT_BUTTON, &PageTestsGPIO::OnEventButton, this);
    Bind(wxEVT_TOGGLEBUTTON, &PageTestsGPIO::OnEventToggleButton, this);
}


wxString PageTestsGPIO::NamePin(Pin::E pin) const
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


int PageTestsGPIO::NumPin(Pin::E pin) const
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


void PageTestsGPIO::OnEventToggleButton(wxCommandEvent &event)
{
    event.Skip();

    int id = event.GetId();

    if (id == btnAutoUART->GetId())
    {
        btnSendUART->Enable(event.GetInt() == 0);

        if (event.GetInt())
        {
            thread_autoUART_is_running = true;

            thread_UART = new std::thread(ThreadFuncAutoUART);

            thread_UART->detach();
        }
        else
        {
            thread_autoUART_is_running = false;

            while (thread_UART->joinable())
            {
            }

            SAFE_DELETE(thread_UART);
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

        FuncEncoder();

        FuncFPGA();
    }
}


void PageTestsGPIO::FuncEncoder()
{
    static bool prevKA = false;
    static bool prevKB = false;

    static bool first = true;

    for (int i = 0; i < 50; i++)
    {
        if (first)
        {
            first = false;
            prevKA = pinKA.Get();
            prevKB = pinKB.Get();
        }
        else
        {
            bool valKA = pinKA.Get();
            bool valKB = pinKB.Get();

            if (valKA != prevKA)
            {
                prevKA = valKA;

                int value = 0;
                PageTestsGPIO::self->txtKA->GetValue().ToInt(&value);
                PageTestsGPIO::self->txtKA->SetValue(wxString::Format("%d", value + 1));
            }

            if (valKB != prevKB)
            {
                prevKB = valKB;

                int value = 0;
                PageTestsGPIO::self->txtKB->GetValue().ToInt(&value);
                PageTestsGPIO::self->txtKB->SetValue(wxString::Format("%d", value + 1));
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


void PageTestsGPIO::FuncFPGA()
{
    static const int SIZE_BUFFER = 8000 / 8 * 4;

    uint8 buffer[SIZE_BUFFER];

    if (pinFIFO_FULL.Get())
    {
        for (int i = 0; i < SIZE_BUFFER * 8; i++)
        {
            pinREQ_RD.ToHi();
            pinDAT_F0.Get();
            pinREQ_RD.ToLow();
        }
    }
}


void PageTestsGPIO::ThreadFuncAutoUART()
{
    while (thread_autoUART_is_running)
    {
        UART::SendByte(0x55);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
    if (!_thread)
    {
        thread_is_running = true;
        _thread = new std::thread(ThreadFunc);
        _thread->detach();
    }
}


void PageTestsGPIO::DeInit()
{
    thread_is_running = false;
 
    if (_thread)
    {
        while (_thread->joinable())
        {
        }

        SAFE_DELETE(_thread);
    }
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

