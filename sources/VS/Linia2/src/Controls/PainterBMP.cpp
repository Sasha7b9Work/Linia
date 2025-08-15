// 2025/08/08 13:57:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/PainterBMP.h"


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
