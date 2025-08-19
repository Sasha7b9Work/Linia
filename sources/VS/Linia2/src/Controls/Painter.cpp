// 2023/09/02 18:49:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Painter.h"
#include "Settings/Color.h"


Painter::Painter(wxWindow *parent, const wxPoint &position, const wxSize &_size) :
    wxPanel(parent, wxID_ANY, position, _size),
    size(_size)
{
    wxPanel::SetSize(size);
    wxPanel::SetDoubleBuffered(true);

    Bind(wxEVT_PAINT, &Painter::OnPaint, this);

    bitmap = new wxBitmap(size);
}


void Painter::BeginPaint()
{
    dc.SelectObject(*bitmap);

    gc = wxGraphicsContext::Create(dc);

    FillRectangle(0, 0, size.x, size.y, GetBackgroundColour());

//    DrawRectangle(0, 0, size.x - 1, size.y - 1, *wxBLACK);
}


void Painter::EndPaint()
{
    dc.SelectObject(wxNullBitmap);

    Refresh();
}


void Painter::FillRectangle(int x, int y, int w, int h, const wxColor &_color)
{
    if (_color != wxNullColour)
    {
        color = _color;
    }

    gc->SetBrush(color);
    gc->SetPen(color);
    gc->DrawRectangle(x, y, w, h);
}


void Painter::DrawRectangle(int x, int y, int w, int h, const wxColor &_color)
{
    if (_color != wxNullColour)
    {
        color = _color;
    }

    gc->SetPen(color);
    gc->SetBrush(wxNullBrush);
    gc->DrawRectangle(x, y, w, h);
}


void Painter::DrawBitmap(int x, int y, const wxBitmap &_bmp, const wxColor &alpha)
{
    wxBitmap bmp = _bmp;

    if (alpha != wxNullColour)
    {
        bmp.SetMask(new wxMask(_bmp, alpha));
    }

    gc->DrawBitmap(bmp, x, y, bmp.GetSize().x, bmp.GetSize().y);
}


void Painter::FillCircle(int x, int y, int r, const wxColor &_color)
{
    if (_color != wxNullColour)
    {
        color = _color;
    }

    gc->SetBrush(color);
    gc->SetPen(color);
    gc->DrawEllipse(x, y, r, r);
}


void Painter::DrawCircle(int x, int y, int r, const wxColor &_color)
{
    if (_color != wxNullColour)
    {
        color = _color;
    }

    gc->SetBrush(wxNullBrush);
    gc->SetPen(color);
    gc->DrawEllipse(x, y, r, r);
}


void Painter::OnPaint(wxPaintEvent &)
{
    wxPaintDC paint_dc(this);

    paint_dc.DrawBitmap(*bitmap, 0, 0);
}


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


PainterBMP::PainterBMP(wxWindow *parent, const wxPoint &position, const wxSize &size, const wxString &file_name, const wxColour &alpha) :
    wxPanel(parent, wxID_ANY, position, size)
{
    bitmap = Bitmap::Get(file_name);

    if (alpha != wxNullColour)
    {
        wxBitmap bmp = bitmap.GetBitmap();
        bmp.SetMask(new wxMask(bitmap.GetBitmap(), alpha));
        bitmap.GetBitmap() = bmp;
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
