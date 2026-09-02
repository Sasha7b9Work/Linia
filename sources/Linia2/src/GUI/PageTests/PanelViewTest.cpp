// 2026/04/29 16:03:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/PanelViewTest.h"


PanelViewTest *ThePanelViewTest = nullptr;


PanelViewTest::PanelViewTest(wxWindow *parent, PanelViewTest *&global) : Panel(parent, wxSIMPLE_BORDER)
{
    global = this;

    Bind(wxEVT_PAINT, &PanelViewTest::OnEventPaint, this);
}


void PanelViewTest::SetTest(Test *_test)
{
    test = _test;

    Refresh();
}


void PanelViewTest::OnEventPaint(wxPaintEvent &event)
{
    if (test)
    {
        wxPaintDC dc(this);

        dc.SetBrush(wxBrush(GetBackgroundColour()));

        dc.SetPen(wxPen(*wxBLACK, 1));

        // Устанавливаем цвет текста
        dc.SetTextForeground(*wxBLACK);

        // Устанавливаем шрифт (опционально)
        dc.SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

        // Рисуем текст в левом верхнем углу
        dc.DrawText(test->lib->name + " : " + test->name, 5, 5);

        DrawElement(dc);
    }

    event.Skip();
}


void PanelViewTest::DrawElement(wxPaintDC &dc)
{
    if (test->lib->UGO == "BJT")
    {
        DrawBJT(dc, "npn", GetCenter(), 100);
    }
    else if (test->lib->UGO == "BJTS")
    {
        DrawBJTS(dc, "npn", GetCenter(), 100);
    }
}


void PanelViewTest::DrawBJT(wxPaintDC &dc, const wxString &type, const wxPoint &c, int radius)
{
    dc.DrawLine(c.x + radius / 2, c.y - 2 * radius, c.x + radius / 2, c.y + 2 * radius);    // Эмиттер

    dc.DrawCircle(c, radius);

    int x_vert = c.x - radius * 10 / 18;                    // Здесь заканчивается линия базы внутри окружности

    dc.DrawLine(c.x - 2 * radius, c.y, x_vert, c.y);        // База

    {
        // Наклонные линии

        int dy = radius * 4 / 18;

        int y_top = c.y - radius * 100 / 115;
        int y_bottom = c.y + radius * 100 / 115;

        int xx = c.x + radius * 10 / 20;

        dc.DrawLine(x_vert, c.y - dy, xx, y_top);            // Верхняя наклонная линия (коллектор)
        dc.DrawLine(x_vert, c.y + dy, xx, y_bottom);         // Нижняя наклонная линия (эмиттер)

        {
            double length = radius * 10 / 40;

            if (type == "npn")
            {
                DrawLineWithAngle(dc, { xx, y_bottom }, length, 125);
                DrawLineWithAngle(dc, { xx, y_bottom }, length, 170);
            }
            else if (type == "pnp")
            {
                DrawLineWithAngle(dc, { x_vert, c.y + dy }, length, -8);
                DrawLineWithAngle(dc, { x_vert, c.y + dy }, length, -53);
            }
            else
            {
                LOG_ERROR("Unknown type transistor");
            }
        }
    }

    {
        // Вертикальная линия

        int dy = radius * 4 / 9;

        dc.DrawLine(x_vert, c.y - dy, x_vert, c.y + dy);
    }
}


void PanelViewTest::DrawBJTS(wxPaintDC &dc, const wxString &type, const wxPoint &c, int radius)
{
    DrawBJT(dc, type, c, radius);
}


void PanelViewTest::DrawLineWithAngle(wxPaintDC &dc, const wxPoint &start, double length, double angleDeg)
{
    double angleRad = angleDeg * M_PI / 180.0;

    int endX = start.x + (int)(length * cos(angleRad));
    int endY = start.y - (int)(length * sin(angleRad));  // минус, т.к. Y вниз

    dc.DrawLine(start.x, start.y, endX, endY);
}


wxPoint PanelViewTest::GetCenter() const
{
    return { 350, 350 };
}
