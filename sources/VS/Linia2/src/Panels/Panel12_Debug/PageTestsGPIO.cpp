// 2025/09/03 09:54:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel12_Debug/PageTestsGPIO.h"
#include "Utils/SystemDepend.h"


PageTestsGPIO *PageTestsGPIO::self = nullptr;


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

        struct StructPin
        {
            PinIn  &in;
            PinOut &out;
        };

        static StructPin pins[9] =
        {
            { pinSTART,      pinOUT1 },
            { pinSTOP,       pinOUT2 },
            { pinDAT_F0,     pinOUT3 },
            { pinDAT_F1,     pinOUT4 },
            { pinDAT_F2,     pinOUT5 },
            { pinDAT_F3,     pinOUT6 },
            { pinREQ_RD,     pinOUT7 },
            { pinFIFO_FULL,  pinOUT8 },
            { pinFIFO_EMPTY, pinOUT9 }
        };

        for (int i = 0; i < 9; i++)
        {
            wxPanel *panel = CreatePanelPin(boxGPIO, pins[i].in, pins[i].out);

            panel->SetPosition({ x, SD::Y_SB(y + i * dy) });
        }
    }
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
        "REQ_RD",
        "FIFO_FULL",
        "FIFO_EMPTY",
        "OUT_1",
        "OUT_2",
        "OUT_3",
        "OUT_4",
        "OUT_5",
        "OUT_6",
        "OUT_7",
        "OUT_8",
        "OUT_9"
    };

    return names[pin];
}


int PageTestsGPIO::NumPin(Pin::E pin) const
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
        1,
        1,
        1,
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


wxPanel *PageTestsGPIO::CreatePanelPin(wxWindow *parent, PinIn &in, PinOut &out)
{
    wxPanel *panel = new wxPanel(parent, wxID_ANY, wxDefaultPosition, { 250, 23 });

    StructGPIO strGPIO;

    strGPIO.button = new wxButton(panel, wxID_ANY, wxString::Format("%s : %d", NamePin(out.GetValue()), NumPin(out.GetValue())), { 0, 0 }, { 70, 22 });

    strGPIO.txtState = new wxTextCtrl(panel, wxID_ANY, "", { 100, 0 }, { 20, 22 });

    new wxStaticText(panel, wxID_ANY, wxString::Format("%s : %d", NamePin(in.GetValue()), NumPin(in.GetValue())), { 150, 2 }, { 100, 22 });

    gpio.push_back(strGPIO);

    return panel;
}
