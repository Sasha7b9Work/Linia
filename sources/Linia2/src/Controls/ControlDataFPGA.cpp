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

    Update();
}


void ControlDataFPGA::SetMax(int _max)
{
    max = _max;

    Update();
}


void ControlDataFPGA::Update()
{
    painter->BeginPaint(*wxLIGHT_GREY);

    painter->gc->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL), *wxBLACK);

    painter->gc->DrawText("0", { 1.0, (double)(painter->GetSize().y - 15) });

    painter->gc->DrawText(wxString::Format("%d", max).ToStdString().c_str(), {1.0, 0.0});

    painter->EndPaint();
}
