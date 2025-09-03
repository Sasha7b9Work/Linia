// 2025/09/03 09:54:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel12_Debug/PageTestsGPIO.h"


PageTestsGPIO *PageTestsGPIO::self = nullptr;


PageTestsGPIO::PageTestsGPIO(wxNotebook *parent) :
    wxPanel(parent)
{
    self = this;

    wxPanel::SetName("Тесты разъёма GPIO");

    new wxStaticBox(this, wxID_ANY, "GPIO", { 10, 10 }, { 100, 100 });

    {
//        int x = 0;
//        int y = 0;
//        int dy = 20;

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
            CreatePanelPin(pins[i].in, pins[i].out);
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


wxPanel *PageTestsGPIO::CreatePanelPin(PinIn &, PinOut &)
{
    return nullptr;
}
