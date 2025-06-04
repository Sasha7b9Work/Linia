// 2023/09/02 18:48:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Color;


class Painter : public wxPanel
{
public:
    Painter(wxWindow *parent, const wxPoint &position, const wxSize &size);

    virtual void OnPaint(wxPaintEvent &);

    void SetColor(const Color &);

    wxColour GetColor() const { return color; }

private:

    wxColour color;
};
