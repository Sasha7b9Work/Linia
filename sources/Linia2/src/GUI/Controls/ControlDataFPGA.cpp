// 2026/04/01 14:24:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/Controls/ControlDataFPGA.h"
#pragma warning(push, 0)
    #include <wx/graphics.h>
#pragma warning(pop)


PainterDataFPGA::PainterDataFPGA(wxWindow *parent, const wxSize &size) :
    Painter(parent, size)
{
}


ControlDataFPGA::ControlDataFPGA(wxWindow *parent, wxStaticText *_for_value) :
    Panel(parent),
    for_value(_for_value)
{
    SetSize(SIZE);

    wxSize size{ SIZE };
    size.x -= 2;
    size.y -= 2;

    painter = new PainterDataFPGA(this, size);

    btnScale = new ToggleButton(painter, L("м"), { 17, 17 });
    btnScale->Bind(wxEVT_TOGGLEBUTTON, &ControlDataFPGA::OnEventToggleButon, this);

    btnScale->SetToolTip(L("Изменение масштаба - автоматический или постоянный"));

    SetMax(32767);

    btnScale->SetPosition({ size.x - btnScale->GetSize().x - 2, 1 });

    Bind(wxEVT_PAINT, &ControlDataFPGA::OnEventPaint, this);
}


void ControlDataFPGA::OnEventToggleButon(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnScale->GetId())
    {
        Draw();
    }
}


void ControlDataFPGA::OnEventPaint(wxPaintEvent &event)
{
    Draw();
    event.Skip();
}


void ControlDataFPGA::SetMax(int _max)
{
    max = _max;

    float scale = (float)_max * 2.0f / (float)(POINTS_IN_SAMPLE_ADC - 1);

    for (int i = 0; i < POINTS_IN_SAMPLE_ADC; i++)
    {
        data[i] = (int)(-(float)max - 1.0f + scale * (float)i + 0.5f);
    }

    Draw();
}


void ControlDataFPGA::Draw()
{
    painter->BeginPaint(*wxWHITE);

    painter->gc->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL), *wxBLACK);

    int min_value = -max - 1;
    int max_value = max;

    if (btnScale->GetValue())
    {
        min_value = max;
        max_value = 0;

        for (int i = 0; i < POINTS_IN_SAMPLE_ADC; i++)
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

    painter->gc->DrawText(wxString::Format("%d", min_value), {0.0, (double)(painter->GetSize().y - 14)});

    painter->gc->DrawText(wxString::Format("%d", max_value), {0.0, -3.0});

    painter->gc->SetPen(*wxBLACK_PEN);
    painter->gc->SetBrush(*wxBLACK_BRUSH);

    float scale_y = (float)(painter->GetSize().y - 4) / (float)(max_value - min_value);

    for (int i = 0; i < painter->GetSize().x; i++)
    {
        int y = (int)((float)(data[i] - min_value) * scale_y) + 2;

        painter->DrawRectangle(i, painter->GetSize().y - y - 1, 1, 1, *wxBLACK);
    }

    painter->EndPaint();

    {
        // Теперь выводим значение, вычисленное по функции

        wxPoint position = GetPosition();
        position.x += SIZE.x;

        for_value->SetPosition(position);
        for_value->SetLabel("Label");
    }
}


void ControlDataFPGA::SetData(int _data[POINTS_IN_SAMPLE_ADC])
{
    std::memcpy(data, _data, sizeof(data[0]) * POINTS_IN_SAMPLE_ADC);

    Draw();
}
