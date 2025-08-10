// 2025/08/08 13:57:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Bitmap.h"


// Отображает BMP


class PainterBMP : public wxPanel
{
public:

    PainterBMP(wxWindow *, const wxPoint &, const wxSize &, const wxString &file_name);

    void SetEnabled(bool);

private:

    Bitmap bitmap;

    void OnEventPaint(wxPaintEvent &);

    bool enabled = true;
};
