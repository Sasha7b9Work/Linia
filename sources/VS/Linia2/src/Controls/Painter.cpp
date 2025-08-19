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


void Painter::Draw()
{
    BeginPaint();

    SetColor(GetBackgroundColour());

    FillRectangle(0, 0, size.x, size.y);

    SetColor(*wxBLACK);

    DrawRectangle(0, 0, size.x - 1, size.y - 1);

    SetColor(*wxWHITE);

    FillRectangle(10, 10, 50, 50);

    SetColor(*wxYELLOW);

    FillCircle(50, 50, 50);

    SetColor(*wxRED);

    DrawCircle(50, 100, 27);

    EndPaint();

    Refresh();
}


void Painter::BeginPaint()
{
    dc.SelectObject(*bitmap);

    gc = wxGraphicsContext::Create(dc);
}


void Painter::EndPaint()
{
    dc.SelectObject(wxNullBitmap);
}


void Painter::SetColor(const wxColor &_color)
{
    color = _color;
}


void Painter::FillRectangle(int x, int y, int w, int h)
{
    gc->SetBrush(color);
    gc->SetPen(color);
    gc->DrawRectangle(x, y, w, h);
}


void Painter::DrawRectangle(int x, int y, int w, int h)
{
    gc->SetPen(color);
    gc->SetBrush(wxNullBrush);
    gc->DrawRectangle(x, y, w, h);
}


void Painter::FillCircle(int x, int y, int r)
{
    gc->SetBrush(color);
    gc->SetPen(color);
    gc->DrawEllipse(x, y, r, r);
}


void Painter::DrawCircle(int x, int y, int r)
{
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
