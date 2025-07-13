// 2022/10/28 23:17:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "MainWindow.h"


class PanelGraph : public Panel
{
public:

    static const int WIDTH = MainWindow::WIDTH2;
    static const int HEIGHT = 500;

    PanelGraph(wxWindow *parent);

    static PanelGraph *self;

private:

    static wxBitmap bitmap;

    void OnPaint(wxPaintEvent &);

    void OnMouseDown(wxMouseEvent &);

    void OnMouseUp(wxMouseEvent &);

    void Init();

    void FillRectangle(int x, int y, int width, int height, const wxColor &);

    void DrawLine(int x1, int y1, int x2, int y2, const wxColor &);

    void DrawString(int x, int y, int font, const wxColor &, pchar text);
};
