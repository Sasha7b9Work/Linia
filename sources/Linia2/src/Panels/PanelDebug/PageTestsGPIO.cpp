// 2025/09/03 09:54:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PageTestsGPIO.h"
#include "Utils/SystemDepend.h"
#include "Communicator/UART/UART.h"
#include "Utils/Timer.h"
#include "MainWindow.h"
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

    wxPanel::SetName("Orange Pi 5");

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
            &pinSPI_MOSI,
            &pinDAT_F2,
            &pinFIFO_FULL,
            nullptr
        };

        pinSTART.SetChangeCallback(CallbackOnStart);

//        pinFIFO_FULL.SetChangeCallback(CallbackOnFIFO_FULL);

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

    wxStaticBox *boxFPGA = new wxStaticBox(this, wxID_ANY, "FPGA", { boxGPIO->GetPosition().x, //-V807
        boxGPIO->GetPosition().y + boxGPIO->GetSize().y + 10 }, { 400, 270 });

    {
        new wxStaticText(boxFPGA, wxID_ANY, "Кол-во измерений", { 10, 20 });
        _txtNumberMeas = new wxTextCtrl(boxFPGA, wxID_ANY, "0", { 150, 20 });

        new wxStaticText(boxFPGA, wxID_ANY, "Кол-во ошибок", { 10, 50 });
        _txtNumberErrors = new wxTextCtrl(boxFPGA, wxID_ANY, "0", { 150, 50 });

        _txtReadData = new wxTextCtrl(boxFPGA, wxID_ANY, " ", { 10, 80 }, { 300, 20 });
    }

    wxStaticBox *boxUART = new wxStaticBox(this, wxID_ANY, "UART", { boxGPIO->GetPosition().x + boxGPIO->GetSize().x + 10, 10 }, { 200, 270 });

    {
        btnReinitUart = new wxButton(boxUART, wxID_ANY, "Reinit", { 10, SD::Y_SB(230) }, { 100, 20 });

        new wxStaticText(boxUART, wxID_ANY, "TX : 8", { 10, SD::Y_SB(20) });
        new wxStaticText(boxUART, wxID_ANY, "RX : 10", { 70, SD::Y_SB(20) });

        txtSendUART = new wxTextCtrl(boxUART, wxID_ANY, "", { 10, SD::Y_SB(50) }, { 170, 20 });
        btnSendUART = new wxButton(boxUART, wxID_ANY, "Send", { 10, SD::Y_SB(75) }, { 100, 20 });
        btnAutoUART = new wxToggleButton(boxUART, wxID_ANY, "AutoSend", { 10, SD::Y_SB(100) }, { 100, 20 });

        new wxStaticText(boxUART, wxID_ANY, "Принято:", { 10, SD::Y_SB(130) });
        txtRecvUART = new wxTextCtrl(boxUART, wxID_ANY, "", { 10, SD::Y_SB(150) }, { 150, 60 }, wxTE_READONLY | wxTE_MULTILINE);
    }

    wxStaticBox *boxSPI = new wxStaticBox(this, wxID_ANY, "SPI", { boxUART->GetPosition().x + boxUART->GetSize().x + 10, 10 }, { 200, 270 });

    {
        new wxStaticText(boxSPI, wxID_ANY, "MOSI : 19", { 10, SD::Y_SB(20) });
        new wxStaticText(boxSPI, wxID_ANY, "CLK : 23", { 10, SD::Y_SB(45) });

        new wxTextCtrl(boxSPI, wxID_ANY, "", { 10, SD::Y_SB(70) }, { 100, 20 });
        btnSendSPI = new wxButton(boxSPI, wxID_ANY, "Send", { 120, SD::Y_SB(70) }, { 50, 20 });
    }

//    CreateBoxEncoder({ boxSPI->GetPosition().x + boxSPI->GetSize().x + 10, 10 });

    wxSize size_button{ 75, BUTTON_HEIGHT };
    btnReturn = new wxButton(this, wxID_ANY, "Закрыть", { MainWindow::WIDTH - size_button.x - 15, 0 }, size_button);

    Bind(wxEVT_BUTTON, &PageTestsGPIO::OnEventButton, this);
    Bind(wxEVT_TOGGLEBUTTON, &PageTestsGPIO::OnEventToggleButton, this);
}


wxStaticBox *PageTestsGPIO::CreateBoxEncoder(const wxPoint &position)
{
    wxStaticBox *boxEncoder = new wxStaticBox(this, wxID_ANY, "Encoder", position, { 200, 270 });

    {
        new wxStaticText(boxEncoder, wxID_ANY, "KA : 11", { 10, SD::Y_SB(20) });
        _txtKA = new wxTextCtrl(boxEncoder, wxID_ANY, "0", { 60, SD::Y_SB(20) }, { 60, 20 }, wxTE_READONLY);

        new wxStaticText(boxEncoder, wxID_ANY, "KB : 13", { 10, SD::Y_SB(45) });
        _txtKB = new wxTextCtrl(boxEncoder, wxID_ANY, "0", { 60, SD::Y_SB(45) }, { 60, 20 }, wxTE_READONLY);
    }

    return boxEncoder;
}


wxString PageTestsGPIO::NamePin(Pin::E pin) const
{
    static const pchar names[Pin::Count] =
    {
        "START",
        "STOP",
        "DAT_F0",
        "SPI_MOSI",
        "DAT_F2",
        "SPI_CS",
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
        wxString message = wxString::Format("%s", PageTestsGPIO::self->txtSendUART->GetValue().c_str().AsChar());

        UART::SendBuffer(message.GetData().AsChar(), (int)(std::strlen(message.GetData().AsChar()) + 1));
    }
    else if (id == btnReturn->GetId())
    {
        MainWindow::self->SetMode(ModeMainWindow::Standard);
    }
    else if (id == btnReinitUart->GetId())
    {
        UART::ReInit();
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
        for (auto &str : PageTestsGPIO::self->gpio_out)
        {
            str.value_pull = str.pin->Get() ? 1 : 0;
        }

        for (auto &str : PageTestsGPIO::self->gpio_in)
        {
            str.value_int = str.pin->Get() ? 1 : 0;
        }

        if (PageTestsGPIO::self->_txtKA)
        {
            ThreadFuncEncoder();
        }

        ThreadFuncFPGA();
    }
}


void PageTestsGPIO::FuncOnRecvUART(char byte)
{
    PageTestsGPIO::self->mutex_str_UART.lock();
    PageTestsGPIO::self->bytesUART.push_back(byte);
    PageTestsGPIO::self->mutex_str_UART.unlock();
}


void PageTestsGPIO::FuncUpdateUART()
{
    std::vector<char> b;
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

            char symbol = b[i];

            if (symbol != 0x00)
            {
                text.Append(symbol);
            }
            else
            {
                PageTestsGPIO::self->txtRecvUART->SetValue(text);
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

    if (PageTestsGPIO::self->_txtKA)
    {
        PageTestsGPIO::self->_txtKA->SetValue(wxString::Format("%d", valueKA));
        PageTestsGPIO::self->_txtKB->SetValue(wxString::Format("%d", valueKB));
    }

    PageTestsGPIO::self->_txtNumberMeas->SetValue(wxString::Format("%d", valueMeas));

    PageTestsGPIO::self->_txtNumberErrors->SetValue(wxString::Format("%d", numErrors));

    PageTestsGPIO::self->_txtReadData->SetValue(wxString::Format("%u %u %u %u, crc=%u, %u ms", values[0], values[1], values[2], values[3], values[4], time_read));

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


uint8 PageTestsGPIO::CalculateCRC(uint16 values[4])
{
    BitSet16 bs0{ values[0] };

    uint8 xor0 = (uint8)(bs0.u8[0] ^ bs0.u8[1]);

    BitSet16 bs1{ values[1] };

    uint8 xor1 = (uint8)(bs1.u8[0] ^ bs1.u8[1]);

    BitSet16 bs2{ values[2] };

    uint8 xor2 = (uint8)(bs2.u8[0] ^ bs2.u8[1]);

    BitSet16 bs3{ values[3] };

    uint8 xor3 = (uint8)(bs3.u8[0] ^ bs3.u8[1]);

    uint8 xor01 = (uint8)(xor0 ^ xor1);
    uint8 xor23 = (uint8)(xor2 ^ xor3);

    uint8 crc = (uint8)(xor01 ^ xor23);

    return crc;
}


void PageTestsGPIO::ThreadFuncFPGA()
{
    static bool prev = false;

    if (pinFIFO_FULL.Get() && prev == false)
    {
        gpiod_line *infoMOSI = GPIO::GetInputPinInfo(Pin::In_SPI_MOSI)->hw.line;
        gpiod_line *infoCS = GPIO::GetOutputPinInfo(Pin::Out_SPI_CS)->hw.line;
        gpiod_line *infoREQ = GPIO::GetOutputPinInfo(Pin::Out_REQ_RD)->hw.line;

        TimeMeterMS meter;

        uint16 values[200][4];

        for (int i = 0; i < 200; i++)
        {
            PinOut::Set(infoCS, 0);

            for (int num_adc = 0; num_adc < 4; num_adc++)
            {
                uint16 value = 0;

//                for (int num_bit = 15; num_bit >= 0; num_bit--)
                for(int num_bit = 0; num_bit < 16; num_bit++)
                {
                    PinOut::Set(infoREQ, 1);

                    if (PinIn::GetHardware(infoMOSI))
                    {
                        value |= (1 << num_bit);
                    }

                    PinOut::Set(infoREQ, 0);
                }

                values[i][num_adc] = value;
            }

            uint8 crc_read = 0;

            for (int num_bit = 0; num_bit < 8; num_bit++)
            {
                PinOut::Set(infoREQ, 1);

                if (PinIn::GetHardware(infoMOSI))
                {
                    crc_read |= (1 << num_bit);
                }

                PinOut::Set(infoREQ, 0);
            }

            PinOut::Set(infoCS, 1);

            uint8 crc = CalculateCRC(values[i]);

            if (crc != crc_read)
            {
                PageTestsGPIO::self->numErrors++;
            }

            if (i == 10)
            {
                for (int j = 0; j < 4; j++)
                {
                    PageTestsGPIO::self->values[j] = values[i][j];
                }
                PageTestsGPIO::self->values[4] = crc_read;
            }
        }

        PageTestsGPIO::self->time_read = (uint)meter.ElapsedMS();

        PageTestsGPIO::self->valueMeas++;
    }

    prev = pinFIFO_FULL.Get();
}


void PageTestsGPIO::ThreadFuncAutoUART()
{
    while (thread_autoUART_is_running)
    {
        wxString message = wxString::Format("%s", PageTestsGPIO::self->txtSendUART->GetValue().c_str().AsChar());

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
        LOG_ERROR("Priority gpio read thread %d is FAIL", priority);
    }
    else
    {
        LOG_WRITE("Priority gpio read thread %d is OK", priority);
    }
}
#endif


void PageTestsGPIO::Init()
{
    if (!_thread)
    {
        thread_is_running = true;
        _thread = new std::thread(ThreadFunc);
#ifdef WIN32
#else
        set_thread_priority_linux(*_thread, SCHED_FIFO, 99);
#endif
        _thread->detach();
    }

    is_init = true;
}


bool PageTestsGPIO::IsInit() const
{
    return is_init;
}


void PageTestsGPIO::DeInit()
{
    is_init = false;

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
    PageTestsGPIO::self->OnChangeStatePin(&pinSPI_MOSI, state);
}

void PageTestsGPIO::CallbackOnDAT_F2(bool state)
{
    PageTestsGPIO::self->OnChangeStatePin(&pinDAT_F2, state);
}

void PageTestsGPIO::CallbackOnDAT_F3(bool /*state*/)
{
//    PageTestsGPIO::self->OnChangeStatePin(&pinDAT_F3, state);
}

void PageTestsGPIO::CallbackOnFIFO_FULL(bool state)
{
    PageTestsGPIO::self->OnChangeStatePin(&pinFIFO_FULL, state);
}

void PageTestsGPIO::CallbackonREQ_RD(bool state)
{
    PageTestsGPIO::self->OnChangeStatePin(&pinREQ_RD, state);
}


void PageTestsGPIO::Pack()
{

}


void PageTestsGPIO::Unpack()
{

}
