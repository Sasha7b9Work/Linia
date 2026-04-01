// 2026/04/01 14:24:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/ControlDataFPGA.h"


PainterDataFPGA::PainterDataFPGA(wxWindow *parent, const wxSize &size) :
    Painter(parent, { 0, 0 }, size)
{
}


ControlDataFPGA::ControlDataFPGA(wxWindow *parent, const wxPoint &position) :
    wxPanel(parent, wxID_ANY, position, SIZE, wxBORDER_SIMPLE)
{
    SetSize(SIZE);

    wxSize size{ SIZE };
    size.x -= 20;
    size.y -= 2;

    painter = new PainterDataFPGA(this, size);

    painter->SetPosition({ 1, 1 });

    btnScale = new wxToggleButton(this, wxID_ANY, "S", { 1 + size.x, 1 }, { 18, 18 });

    btnScale->SetToolTip("Изменение масштаба - автоматический или постоянный");
}


void ControlDataFPGA::SetMax(int _max)
{
    max = _max;

    Update();
}


void ControlDataFPGA::Update()
{
    painter->Refresh();
}
