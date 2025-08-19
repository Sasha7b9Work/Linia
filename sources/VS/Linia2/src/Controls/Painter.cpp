// 2023/09/02 18:49:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Painter.h"
#include "Settings/Color.h"


PainterRect::PainterRect(wxWindow *parent, const wxPoint &position, const wxSize &size) :
    wxPanel(parent, wxID_ANY, position, size)
{
    wxPanel::SetSize(size);
    wxPanel::SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &PainterRect::OnPaint, this);

    color = wxColour(255U, 0, 0);
}


void PainterRect::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(wxBrush(color));
    dc.DrawRectangle(0, 0, GetSize().x - 1, GetSize().y - 1);
}


void PainterRect::SetColor(const Color &_color)
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


PainterBMP::PainterBMP(wxWindow *parent, const wxPoint &position, const wxSize &size, const wxString &file_name, wxColour *transparent) :
    wxPanel(parent, wxID_ANY, position, size)
{
    bitmap = Bitmap::Get(file_name);

    if (transparent)
    {
        wxBitmap bmp = bitmap.GetBitmap();
        bmp.SetMask(new wxMask(bitmap.GetBitmap(), *transparent));
        bitmap.GetBitmap() = bmp;
        delete transparent;
    }

    if (size == wxDefaultSize)
    {
        SetSize(bitmap.GetBitmap().GetSize());
    }

    Bind(wxEVT_PAINT, &PainterBMP::OnEventPaint, this);

    Refresh();
}


void PainterBMP::OnEventPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    if (bitmap.GetBitmap().IsOk())
    {
        dc.DrawBitmap(bitmap.GetBitmap(), 0, 0, true);
    }
}
