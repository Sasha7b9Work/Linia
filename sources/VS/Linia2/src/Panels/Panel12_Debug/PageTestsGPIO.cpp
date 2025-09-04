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

    wxStaticBox *boxGPIO = new wxStaticBox(this, wxID_ANY, "GPIO", { 10, 10 }, { 300, 300 });

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

        int index = 0;

        PinIn *in = pins_in[index];

        while (in)
        {
            wxPanel *panel = CreatePanelPinIn(boxGPIO, *in);

            panel->SetPosition({ x, y });

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
            wxPanel *panel = CreatePanelPinOut(boxGPIO, *out);

            panel->SetPosition({ x, y });

            y += dy;

            out = pins_out[++index];
        }
    }
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


wxPanel *PageTestsGPIO::CreatePanelPinOut(wxWindow *parent, PinOut &pin)
{
    wxPanel *panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, { 250, 23 });

    StructOutGPIO strGPIO(pin);

    strGPIO.button = new wxButton(panel, wxID_ANY, NamePin(pin.type()), { 0, 0 }, { 70, 22 });

    strGPIO.button->Bind(wxEVT_BUTTON, &PageTestsGPIO::OnEventButton, this);

    strGPIO.txtState = new wxTextCtrl(panel, wxID_ANY, "", { 100, 0 }, { 20, 22 }, wxTE_READONLY);

    new wxStaticText(panel, wxID_ANY, wxString::Format("%d", NumPin(pin.type())), {130, 0});

    gpio_out.push_back(strGPIO);

    return panel;
}


wxPanel *PageTestsGPIO::CreatePanelPinIn(wxWindow *parent, PinIn &pin)
{
    wxPanel *panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, { 250, 23 });

    StructInGPIO strGPIO(pin);

    new wxStaticText(panel, wxID_ANY, NamePin(pin.type()), { 0, 0 }, { 80, 22 });

    strGPIO.txtState = new wxTextCtrl(panel, wxID_ANY, "", { 100, 0 }, { 20, 22 }, wxTE_READONLY);

    new wxStaticText(panel, wxID_ANY, wxString::Format("%d", NumPin(pin.type())), { 130, 0 });

    gpio_in.push_back(strGPIO);

    return panel;
}


void PageTestsGPIO::OnEventButton(wxCommandEvent & /*event*/)
{

}


void PageTestsGPIO::ThreadFunc()
{
    while (thread_is_running)
    {
        for (auto &str : PageTestsGPIO::self->gpio_out)
        {
            str.txtState->SetValue(str.pin.Get() ? "1" : "0");
        }

        for (auto &str : PageTestsGPIO::self->gpio_in)
        {
            str.txtState->SetValue(str.pin.Get() ? "1" : "0");
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
