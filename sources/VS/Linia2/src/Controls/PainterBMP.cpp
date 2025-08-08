// 2025/08/08 13:57:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/PainterBMP.h"


PainterBMP::PainterBMP(wxWindow *parent, const wxPoint &position, const wxSize &size, const wxString &file_name) :
    wxPanel(parent, wxID_ANY, position, size)
{
    if (!bitmap.LoadFile(file_name, wxBITMAP_TYPE_BMP))
    {
        LOG_ERROR("Не удалось загрузить файл изображения %s", file_name.c_str().AsChar());
    }

    Bind(wxEVT_PAINT, &PainterBMP::OnEventPaint, this);
}


void PainterBMP::OnEventPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    if (bitmap.IsOk())
    {
        dc.DrawBitmap(bitmap, 0, 0, true);
    }
}
