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

    new wxStaticBox(this, wxID_ANY, "GPIO", { 10, 10 }, { 300, 300 });

    {
//        int x = 10;
//        int y = 20;
//        int dy = 31;

        struct StructPin
        {
            PinIn  &in;
            PinOut &out;
        };

//        static StructPin pins[9] =
//        {
//            { pinSTART,      pinOUT1 },
//            { pinSTOP,       pinOUT2 },
//            { pinDAT_F0,     pinOUT3 },
//            { pinDAT_F1,     pinOUT4 },
//            { pinDAT_F2,     pinOUT5 },
//            { pinDAT_F3,     pinOUT6 },
//            { pinREQ_RD,     pinOUT7 },
//            { pinFIFO_FULL,  pinOUT8 },
//            { pinFIFO_EMPTY, pinOUT9 }
//        };
//
//        for (int i = 0; i < 9; i++)
//        {
//            wxPanel *panel = CreatePanelPin(boxGPIO, pins[i].in, pins[i].out);
//
//            panel->SetPosition({ x, SD::Y_SB(y + i * dy) });
//        }
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
        "REQ_RD",
        "FIFO_FULL"
    };

    return names[pin];
}


int PageTestsGPIO::NumPin(Pin::Type pin) const
{
    static const int num[Pin::Count] =
    {
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1
    };

    return num[pin];
}


wxPanel *PageTestsGPIO::CreatePanelPin(wxWindow *parent, PinIn & /*in*/, PinOut & /*out*/)
{
    wxPanel *panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, { 250, 23 });

//    StructGPIO strGPIO(in, out);
//
//    strGPIO.button = new wxButton(panel, wxID_ANY, wxString::Format("%s : %d", NamePin(out.GetValue()), NumPin(out.GetValue())), { 0, 0 }, { 70, 22 });
//
//    strGPIO.button->Bind(wxEVT_BUTTON, &PageTestsGPIO::OnEventButton, this);
//
//    strGPIO.txtState = new wxTextCtrl(panel, wxID_ANY, "", { 100, 0 }, { 20, 22 }, wxTE_READONLY);
//
//    new wxStaticText(panel, wxID_ANY, wxString::Format("%s : %d", NamePin(in.GetValue()), NumPin(in.GetValue())), { 150, 2 }, { 100, 22 });
//
//    gpio.push_back(strGPIO);

    return panel;
}


void PageTestsGPIO::OnEventButton(wxCommandEvent & /*event*/)
{

}


void PageTestsGPIO::ThreadFunc()
{
    while (thread_is_running)
    {
        for (auto &str : PageTestsGPIO::self->gpio)
        {
            str.txtState->SetValue(str.in.Get() ? "1" : "0");
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
