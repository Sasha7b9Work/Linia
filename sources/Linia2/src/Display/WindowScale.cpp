// 2025/7/25 10:02:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/SystemDepend.h"
#include "Display/WindowScale.h"
#include "Controls/StaticBox.h"


WindowScale::Range WindowScale::rangeX{ -12, 28, "Uc", "V"};
WindowScale::Range WindowScale::rangeY{ -2.5, 7.5, "Ic", "A"};


WindowScale::WindowScale() :
    Dialog(nullptr, wxID_ANY, "Шкала", wxDefaultPosition, { WIDTH, HEIGHT })
{
    int width = 200;
    int height = 200;

    int d = 10;

    CreateBox(d, d, width, height, "X", "Ud, V", &textMinX, &textMaxX);

    CreateBox(width + d * 2, d, width, height, "Y", "Id, mA", &textMinY, &textMaxY);

    int y = height + d * 3;

    new wxStaticText(this, wxID_ANY, "Точность", { d, y });

    new wxSpinCtrl(this, wxID_ANY, "", { 100, y }, { 70, TEXTCNTRL_HEIGHT});

    y += 50;

    wxArrayString choices;
    choices.Add("20");
    choices.Add("50");

    y += 50;

    wxSize size_button{ 100, BUTTON_HEIGHT };

    btnApply = new wxButton(this, wxID_ANY, "Применить", { 50, y }, size_button);

    btnCancel = new wxButton(this, wxID_ANY, "Отмена", { 150, y }, size_button);
}


wxStaticBox *WindowScale::CreateBox(int x, int y, int w, int h, pchar axe, pchar units, wxTextCtrl **min, wxTextCtrl **max)
{
    StaticBox *box = new StaticBox(this, wxString("Шкала по оси ") + axe, {x, y}, {w, h});

    y = 25;

    int d = 6;

    int dy = 35;

    new wxStaticText(box, wxID_ANY, units, { d / 2, SD::Y_SB(y) }, { w - d, 20 }, wxALIGN_CENTER);

    y += dy;

    new wxStaticText(box, wxID_ANY, "X min", { d, SD::Y_SB(y) });

    int s = 50;

    wxSize size_text{ 50, TEXTCNTRL_HEIGHT };

    *min = new wxTextCtrl(box, wxID_ANY, "", { d + s, SD::Y_SB(y) }, size_text);

    y += dy;

    new wxStaticText(box, wxID_ANY, "X max", { d, SD::Y_SB(y) });

    *max = new wxTextCtrl(box, wxID_ANY, "", { d + s, SD::Y_SB(y) }, size_text);

    y += dy * 3 / 2;

    new wxStaticText(box, wxID_ANY, "Шаг", { 10, SD::Y_SB(y) });

    return box;
}


double WindowScale::Range::MaxAbs() const
{
    double _min = std::fabs(min);
    double _max = std::fabs(max);

    return (_min > _max) ? _min : _max;
}


double WindowScale::Range::Amplitude() const
{
    return max - min;
}


void WindowScale::Range::operator+=(const double &delta)
{
    min += delta;
    max += delta;
}


void WindowScale::Range::operator*=(const double &delta)
{
    double center = (max + min) / 2.0;

    double amplitude = Amplitude();

    amplitude *= delta;

    min = center - amplitude / 2.0;
    max = center + amplitude / 2.0;
}


wxString WindowScale::Range::FullTitle() const
{
    wxString prefix;

    if (MaxAbs() >= 1e3)
    {
        prefix = "k";
    }
    else if (MaxAbs() >= 1.0)
    {

    }
    else if (MaxAbs() >= 1e-3)
    {
        prefix = "m";
    }
    else if (MaxAbs() >= 1e-6)
    {
        prefix = "u";
    }
    else if (MaxAbs() >= 1e-9)
    {
        prefix = "n";
    }

    return title + "," + prefix + units;
}


wxString WindowScale::Range::GetValuePointAxis(int num) const
{
    double step = Amplitude() / 10.0;   // По горизонтали всегда 10 клеток

    if (MaxAbs() >= 1e3)
    {
        step /= 1e3;
    }
    else if (MaxAbs() >= 1)
    {

    }
    else if (MaxAbs() >= 1e-3)
    {
        step *= 1e3;
    }
    else if (MaxAbs() >= 1e-6)
    {
        step *= 1e6;
    }
    else
    {
        step *= 1e9;
    }

    return wxString::Format("%.1f", step * num);
}
