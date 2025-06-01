// 2023/09/02 18:49:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Painter.h"
#include "Settings/Color.h"


Painter::Painter(wxWindow *parent, wxPoint position, wxSize size, TypePainter::E _type) :
    wxPanel(parent, wxID_ANY, position, size),
    type(_type)
{
    wxPanel::SetSize(size);
    wxPanel::SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &Painter::OnPaint, this);

    color = (type == 0) ? wxColour(255U, 0, 0) : wxColour(0, 255U, 0);

    if (type == TypePainter::Eye)
    {
        color = wxColour(255, 255, 255);
    }

    Bind(wxEVT_LEFT_UP, &Painter::OnEventMouseLeft, this);
}


void Painter::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(wxBrush(color));
    dc.DrawRectangle(0, 0, GetSize().x - 1, GetSize().y - 1);
}


void Painter::SetColor(const Color &_color)
{
    float k = _color.GetBrightnessF();

    uint red = (uint8)((float)_color.GetRed() * k);
    uint green = (uint8)((float)_color.GetGreen() * k);
    uint blue = (uint8)((float)_color.GetBlue() * k);

    color.Set(
        (wxColourBase::ChannelType)red,
        (wxColourBase::ChannelType)green,
        (wxColourBase::ChannelType)blue
    );

    Refresh(true);
}


void Painter::OnEventMouseLeft(wxMouseEvent &event)
{
    if (event.IsButton())
    {
        GetParent()->ProcessWindowEvent(event);
        GetParent()->SetFocus();
    }
}
