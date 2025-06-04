// 2023/09/02 18:48:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Color;


struct TypePainter
{
    enum E
    {
        Red,        // Рисует квадратик цвета "Red"
        Green,      // Рисует квадратик цвета "Green"
        Eye,
        Color_,     // Поле в диалоге выбора цвета
        Count
    };
};


class Painter : public wxPanel
{
public:
    Painter(wxWindow *parent, wxPoint position, wxSize size, TypePainter::E _type);

    virtual void OnPaint(wxPaintEvent &);

    void SetColor(const Color &);

    wxColour GetColor() const { return color; }

private:

    TypePainter::E type;
    wxColour color;
};
