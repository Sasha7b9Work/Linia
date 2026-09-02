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
        wxColor color_back = GetBackgroundColour();

        wxPaintDC dc(this);

        dc.SetBrush(*wxTRANSPARENT_BRUSH);

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
        dc.DrawCircle(GetCenter(), 100);
    }
}


wxPoint PanelViewTest::GetCenter() const
{
    return { 350, 350 };
}
