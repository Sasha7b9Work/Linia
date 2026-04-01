// 2026/04/01 14:24:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/ControlDataFPGA.h"


PainterDataFPGA::PainterDataFPGA(wxWindow *parent) : Painter(parent, { 0, 0 }, { 100, 100 })
{

}


ControlDataFPGA::ControlDataFPGA(wxWindow *parent, const wxPoint &position) :
    wxPanel(parent, wxID_ANY, position, SIZE),
    painter{ this }
{

}
