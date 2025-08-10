// 2025/08/08 13:57:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/PainterBMP.h"


PainterBMP::PainterBMP(wxWindow *parent, const wxPoint &position, const wxSize &size, const wxString &file_name) :
    wxPanel(parent, wxID_ANY, position, size)
{
    bitmap = Bitmap::Get(file_name);

    Bind(wxEVT_PAINT, &PainterBMP::OnEventPaint, this);

    Refresh();
}


void PainterBMP::OnEventPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    if (bitmap.GetBitmap().IsOk())
    {
        if (enabled)
        {
            dc.DrawBitmap(bitmap.GetBitmap(), 0, 0, true);
        }
        else
        {
            wxImage image = bitmap.GetBitmap().ConvertToImage();

            const wxColour white = *wxWHITE;
            wxColour grey(220, 220, 220);

            image.Replace(white.Red(), white.Green(), white.Blue(),
                grey.Red(), grey.Green(), grey.Blue());

            wxColour black(4, 4, 4);            // Линии
            grey = { 100, 100, 100 };

            image.Replace(black.Red(), black.Green(), black.Blue(),
                grey.Red(), grey.Green(), grey.Blue());

            black = { 165, 0, 33 };             // Надписи

            image.Replace(black.Red(), black.Green(), black.Blue(),
                grey.Red(), grey.Green(), grey.Blue());

            wxBitmap modified(image);

            dc.DrawBitmap(modified, 0, 0, false);
        }
    }
}


void PainterBMP::SetEnabled(bool en)
{
    if (en != enabled)
    {
        enabled = en;

        Refresh();
    }
}
