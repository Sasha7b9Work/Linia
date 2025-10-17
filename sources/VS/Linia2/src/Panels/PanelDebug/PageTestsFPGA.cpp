// 2025/10/13 13:54:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PageTestsFPGA.h"


PageTestsFPGA *PageTestsFPGA::self = nullptr;


PageTestsFPGA::PageTestsFPGA(wxNotebook *parent) :
    wxPanel(parent)
{
    self = this;

    wxPanel::SetName("FPGA");
}


void PageTestsFPGA::Pack()
{

}


void PageTestsFPGA::Unpack()
{

}
