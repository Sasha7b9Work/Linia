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

    btnScale = new wxToggleButton(this, wxID_ANY, "S", { 1 + size.x, 1 }, { 17, 17 });

    btnScale->SetToolTip("Изменение масштаба - автоматический или постоянный");

    Draw();
}


void ControlDataFPGA::SetMax(int _max)
{
    max = _max;

    Draw();
}


void ControlDataFPGA::Draw(int data[200])
{
    painter->BeginPaint(*wxLIGHT_GREY);

    painter->gc->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL), *wxBLACK);

    int min_value = 0;
    int max_value = max;

    if (btnScale->GetValue() && data)
    {
        min_value = max;
        max_value = 0;

        for (int i = 0; i < 200; i++)
        {
            if (data[i] < min_value)
            {
                min_value = data[i];
            }
            if (data[i] > max_value)
            {
                max_value = data[i];
            }
        }
    }

    painter->gc->DrawText(wxString::Format("%d", min_value), {0.0, (double)(painter->GetSize().y - 15)});

    painter->gc->DrawText(wxString::Format("%d", max_value), {0.0, -3.0});

    if (data)
    {
        painter->gc->SetBrush(*wxBLACK_BRUSH);

        float scale_y = (float)painter->GetSize().y / (float)(max_value - min_value);

        for (int i = 0; i < painter->GetSize().x; i++)
        {
            int y = (int)((float)(data[i] - min_value) * scale_y);

            painter->gc->DrawRectangle(i, y, 1, 1);
        }
    }

    painter->EndPaint();
}


void ControlDataFPGA::SetData(int data[200])
{
    Draw(data);
}
