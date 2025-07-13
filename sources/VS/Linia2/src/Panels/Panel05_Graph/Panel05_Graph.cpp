// 2022/10/28 23:17:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel05_Graph/Panel05_Graph.h"
#include "MainWindow.h"
#include <string>
#include <map>
#include <algorithm>


wxBitmap PanelGraph::bitmap(WIDTH, HEIGHT);


PanelGraph *PanelGraph::self = nullptr;

static const int width_button = 130;
static const int height_button = 72;
static const int y_button = 405;
static const int x_wave = 9;
static const int y_wave = 10;
static const int width_wave = 390;
static const int height_wave = 196;


PanelGraph::PanelGraph(wxWindow *parent) :
    Panel(parent, MainWindow::WIDTH1, MainWindow::HEIGTH1, WIDTH, HEIGHT)
{
    self = this;

    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &PanelGraph::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &PanelGraph::OnMouseDown, this);
    Bind(wxEVT_LEFT_UP, &PanelGraph::OnMouseUp, this);

    Draw();
}


void PanelGraph::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    dc.DrawBitmap(bitmap, 0, 0);
}


void PanelGraph::OnMouseDown(wxMouseEvent &) //-V2009
{
}


void PanelGraph::OnMouseUp(wxMouseEvent &)
{
}


void PanelGraph::Draw()
{
    FillRectangle(0, 0, WIDTH, HEIGHT, *wxWHITE);

    grid.Draw();
}


void PanelGraph::FillRectangle(int x, int y, int width, int height, const wxColor &color)
{
    wxMemoryDC dc;
    dc.SelectObject(bitmap);
    dc.SetBrush(color);
    dc.SetPen(color);
    dc.DrawRectangle({ x, y, width, height });
    dc.SelectObject(wxNullBitmap);
    Refresh();
}


void PanelGraph::DrawLine(int x1, int y1, int x2, int y2)
{
    wxMemoryDC dc;
    dc.SelectObject(bitmap);
    dc.DrawLine(x1, y1, x2, y2);
    dc.SelectObject(wxNullBitmap);
    Refresh();
}


void PanelGraph::DrawLine(int x1, int y1, int x2, int y2, const wxColor &color)
{
    wxMemoryDC dc;
    dc.SelectObject(bitmap);
    dc.SetPen(color);
    dc.DrawLine(x1, y1, x2, y2);
    dc.SelectObject(wxNullBitmap);
    Refresh();
}


void PanelGraph::DrawString(int x, int y, int /*num_font*/, const wxColor &color, pchar text)
{
    wxMemoryDC dc;
    dc.SelectObject(bitmap);
    dc.SetTextForeground(color);
//    dc.SetFont(FontGUI::Get(num_font));
    dc.DrawText(text, x + 5, y + 5);
    dc.SelectObject(wxNullBitmap);
    Refresh();
}
