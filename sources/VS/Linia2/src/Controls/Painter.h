// 2023/09/02 18:48:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Bitmap.h"


struct Color;


class Painter : public wxPanel
{
public:

    Painter(wxWindow *parent, const wxPoint &position, const wxSize &size);

    virtual void OnPaint(wxPaintEvent &);

    void Draw();

    void BeginPaint();
    void EndPaint();
    void SetColor(const wxColor &);
    void FillCircle(int x, int y, int r, const wxColor & = wxNullColour);
    void DrawCircle(int x, int y, int r);
    void FillRectangle(int x, int y, int w, int h);
    void DrawRectangle(int x, int y, int w, int h);

private:

    wxBitmap *bitmap = nullptr;
    wxGraphicsContext *gc = nullptr;
    wxMemoryDC dc;
    wxColor color;
    wxSize size;
};


// Тупо заливает на себе прямоугольник
class PainterRect : public wxPanel
{
public:

    PainterRect(wxWindow *parent, const wxPoint &position, const wxSize &size);

    virtual void OnPaint(wxPaintEvent &);

    void SetColor(const Color &);

    wxColour GetColor() const { return color; }

private:

    wxColour color;
};



// Отображает BMP
class PainterBMP : public wxPanel
{
public:

    // Если transparentColour != nullptr, то пиксели данного цвета будут прозрачными
    PainterBMP(wxWindow *, const wxPoint &, const wxSize &, const wxString &file_name, wxColour *transparentColour = nullptr);

private:

    Bitmap bitmap;

    void OnEventPaint(wxPaintEvent &);
};
