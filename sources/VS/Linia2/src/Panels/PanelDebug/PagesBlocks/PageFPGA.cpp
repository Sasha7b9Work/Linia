// 2025/10/13 13:54:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PagesBlocks/PageFPGA.h"


PageFPGA *PageFPGA::self = nullptr;


PageFPGA::PageFPGA(wxNotebook *parent) :
    wxPanel(parent)
{
    self = this;

    wxPanel::SetName("FPGA");
}


void PageFPGA::Pack()
{

}


void PageFPGA::Unpack()
{

}
