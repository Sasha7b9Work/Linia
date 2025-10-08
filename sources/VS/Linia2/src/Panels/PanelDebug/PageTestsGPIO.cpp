// 2025/09/03 09:54:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PageTestsGPIO.h"
#include "Utils/SystemDepend.h"
#include "Communicator/UART/UART.h"
#include "Utils/Timer.h"
#ifdef WIN32
#else
    #include <sched.h>
#endif


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
        _txtNumberMeas = new wxTextCtrl(boxFPGA, wxID_ANY, "0", { 150, 20 });

        new wxStaticText(boxFPGA, wxID_ANY, "Кол-во ошибок", { 10, 50 });
        _txtNumberErrors = new wxTextCtrl(boxFPGA, wxID_ANY, "0", { 150, 50 });
    }

    wxStaticBox *boxUART = new wxStaticBox(this, wxID_ANY, "UART", { boxGPIO->GetPosition().x + boxGPIO->GetSize().x + 10, 10 }, { 200, 270 });

    {
        new wxStaticText(boxUART, wxID_ANY, "TX : 8", { 10, SD::Y_SB(20) });
        new wxStaticText(boxUART, wxID_ANY, "RX : 10", { 70, SD::Y_SB(20) });

        txtSendUART = new wxTextCtrl(boxUART, wxID_ANY, "", { 10, SD::Y_SB(50) }, { 170, 20 });
        btnSendUART = new wxButton(boxUART, wxID_ANY, "Send", { 10, SD::Y_SB(75) }, { 100, 20 });
        btnAutoUART = new wxToggleButton(boxUART, wxID_ANY, "AutoSend", { 10, SD::Y_SB(100) }, { 100, 20 });

        new wxStaticText(boxUART, wxID_ANY, "Принято:", { 10, SD::Y_SB(150) });
        txtRecvUART = new wxTextCtrl(boxUART, wxID_ANY, "", { 10, SD::Y_SB(170) }, { 170, 20 }, wxTE_READONLY);
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
        _txtKA = new wxTextCtrl(boxEncoder, wxID_ANY, "0", { 50, 20 }, { 60, 20 }, wxTE_READONLY);

        new wxStaticText(boxEncoder, wxID_ANY, "KB : 13", { 10, 45 });
        _txtKB = new wxTextCtrl(boxEncoder, wxID_ANY, "0", { 50, 45 }, { 60, 20 }, wxTE_READONLY);
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

    strGPIO._txtStatePull = new wxTextCtrl(panel, wxID_ANY, "", { 100, 0 }, { 20, 22 }, wxTE_READONLY);

    strGPIO._txtStateInt = new wxTextCtrl(panel, wxID_ANY, "", { 130, 0 }, { 20, 22 }, wxTE_READONLY);

    new wxStaticText(panel, wxID_ANY, wxString::Format("%d", NumPin(pin->type())), { 160, 2 }, { 40, 22 });

    gpio_out.push_back(strGPIO);

    return panel;
}


wxPanel *PageTestsGPIO::CreatePanelPinIn(wxWindow *parent, PinIn *pin)
{
    wxPanel *panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, { 180, 23 });

    StructInGPIO strGPIO(pin);

    new wxStaticText(panel, wxID_ANY, NamePin(pin->type()), { 0, 2 }, { 80, 22 });

    strGPIO._txtStatePull = new wxTextCtrl(panel, wxID_ANY, "", { 100, 0 }, { 20, 22 }, wxTE_READONLY);

    strGPIO._txtStateInt = new wxTextCtrl(panel, wxID_ANY, "", { 130, 0 }, { 20, 22 }, wxTE_READONLY);

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
        bool enable = event.GetInt() == 0;

        btnSendUART->Enable(enable);
        txtSendUART->Enable(enable);

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
//        for (auto &str : PageTestsGPIO::self->gpio_out)
//        {
//            str.value_pull = str.pin->Get() ? 1 : 0;
//        }
//
//        for (auto &str : PageTestsGPIO::self->gpio_in)
//        {
//            str.value_int = str.pin->Get() ? 1 : 0;
//        }
//
//        ThreadFuncEncoder();

        ThreadFuncFPGA();
    }
}


void PageTestsGPIO::FuncRecvUART(uint8 byte)
{
    PageTestsGPIO::self->mutex_str_UART.lock();
    PageTestsGPIO::self->bytesUART.push_back(byte);
    PageTestsGPIO::self->mutex_str_UART.unlock();
}


void PageTestsGPIO::FuncUpdateUART()
{
    std::vector<uint8> b;
    PageTestsGPIO::self->mutex_str_UART.lock();
    if (PageTestsGPIO::self->bytesUART.size())
    {
        b = PageTestsGPIO::self->bytesUART;
        PageTestsGPIO::self->bytesUART.clear();
    }
    PageTestsGPIO::self->mutex_str_UART.unlock();

    if (b.size())
    {
        for (uint i = 0; i < b.size(); i++)
        {
            static wxString text;

            char symbol = (char)b[i];

            if (symbol != 0x00)
            {
                text.Append(symbol);
            }
            else
            {
                PageTestsGPIO::self->txtRecvUART->SetValue(text);
                text.Clear();
            }

            if (text.Length() > 50)
            {
                text.Clear();
            }
        }
    }
}


void PageTestsGPIO::Update()
{
    for (auto &str : PageTestsGPIO::self->gpio_out)
    {
        str._txtStatePull->SetValue(wxString::Format("%d", str.value_pull));
    }

    for (auto &str : PageTestsGPIO::self->gpio_in)
    {
        str._txtStatePull->SetValue(wxString::Format("%d", str.value_int));
    }

    PageTestsGPIO::self->_txtKA->SetValue(wxString::Format("%d", valueKA));

    PageTestsGPIO::self->_txtKB->SetValue(wxString::Format("%d", valueKB));

    PageTestsGPIO::self->_txtNumberMeas->SetValue(wxString::Format("%d", valueMeas));

    PageTestsGPIO::self->_txtNumberErrors->SetValue(wxString::Format("%d", numErrors));

    FuncUpdateUART();
}


void PageTestsGPIO::ThreadFuncEncoder()
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

                PageTestsGPIO::self->valueKA++;
            }

            if (valKB != prevKB)
            {
                prevKB = valKB;

                PageTestsGPIO::self->valueKB++;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


void PageTestsGPIO::ThreadFuncFPGA()
{
    static bool prev = false;

    if (pinFIFO_FULL.Get() && prev == false)
    {
        TimeMeterMS meter;

        int bytes_left = 8000;

        uint8 bytes[5];

        bool _error = false;

        while (bytes_left > 0)
        {
            pinSPI_CS.ToHi();

            for (int b = 0; b < 5; b++)
            {
                uint8 byte = 0;

                for (int i = 7; i >= 0; i--)
                {
                    bytes_left--;
                    pinREQ_RD.ToHi();
                    if (pinDAT_F0.Get())
                    {
                        byte |= (1 << i);
                    }
                    else
                    {
                        byte |= 0;
                    }
                    pinREQ_RD.ToLow();
                }

                bytes[b] = byte;
            }

            pinSPI_CS.ToLow();

            uint8 crc = (uint8)(bytes[0] ^ bytes[1] ^ bytes[2] ^ bytes[3]);

            if (crc != bytes[4])
            {
                _error = true;
            }
        }

        PageTestsGPIO::self->valueMeas++;

        if (_error)
        {
            PageTestsGPIO::self->numErrors++;
        }

//        LOG_WRITE("Time FPGA = %f ms", meter.ElapsedTime());
    }

    prev = pinFIFO_FULL.Get();
}


void PageTestsGPIO::ThreadFuncAutoUART()
{
    while (thread_autoUART_is_running)
    {
        static int counter = 0;

        wxString message = wxString::Format("%s %d", PageTestsGPIO::self->txtSendUART->GetValue().c_str().AsChar(), counter++);

        UART::SendBuffer(message.GetData().AsChar(), (int)(std::strlen(message.GetData().AsChar()) + 1));

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


void PageTestsGPIO::OnChangeStatePin(PinIn *pin, bool state)
{
    for (auto &str : PageTestsGPIO::self->gpio_in)
    {
        if (str.pin == pin)
        {
            str._txtStateInt->SetValue(state ? "1" : "0");
        }
    }
}


void PageTestsGPIO::OnChangeStatePin(PinOut *pin, bool state)
{
    for (auto &str : PageTestsGPIO::self->gpio_out)
    {
        if (str.pin == pin)
        {
            str._txtStateInt->SetValue(state ? "1" : "0");
        }
    }
}


#ifdef WIN32
#else
void set_thread_priority_linux(std::thread &thread, int policy, int priority)
{
    pthread_t handle = thread.native_handle();

    sched_param sch_params;
    sch_params.sched_priority = priority;

    if (pthread_setschedparam(handle, policy, &sch_params) != 0)
    {
        std::cerr << "Failed to set thread priority" << std::endl;
    }
}
#endif


void PageTestsGPIO::Init()
{
    if (!_thread)
    {
        UART::RecvCallback::Store();
        UART::RecvCallback::Set(FuncRecvUART);

        thread_is_running = true;
        _thread = new std::thread(ThreadFunc);
#ifdef WIN32
#else
        set_thread_priority_linux(t, SCHED_FIFO, 50);
#endif
        _thread->detach();
    }
}


void PageTestsGPIO::DeInit()
{
    thread_is_running = false;
 
    if (_thread)
    {
        UART::RecvCallback::Restore();

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
