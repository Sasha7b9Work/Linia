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
    Bind(wxEVT_MOTION, &PanelGraph::OnMouseMove, this);
    Bind(wxEVT_MOUSEWHEEL, &PanelGraph::OnMouseWheel, this);

    Draw();
}


void PanelGraph::OnPaint(wxPaintEvent &)
{
    wxPaintDC paint_dc(this);

    paint_dc.DrawBitmap(bitmap, 0, 0);
}


void PanelGraph::OnMouseDown(wxMouseEvent &event)
{
    pos_mouse_down = event.GetPosition();

    mouse_is_pressed = true;
}


void PanelGraph::OnMouseUp(wxMouseEvent &)
{
    mouse_is_pressed = false;
}


void PanelGraph::OnMouseMove(wxMouseEvent &event)
{
    if (!mouse_is_pressed)
    {
        return;
    }

    wxPoint position = event.GetPosition();

    wxPoint delta = position - pos_mouse_down;

    grid.MoveOn(delta);

    pos_mouse_down = position;

    Draw();
}


void PanelGraph::OnMouseWheel(wxMouseEvent &event)
{
    grid.ScaleOn(event.GetPosition(), event.GetWheelRotation());

    Draw();
}


void PanelGraph::Draw()
{
    BeginPaint();

    FillRectangle(0, 0, WIDTH, HEIGHT, *wxWHITE);

    grid.Draw();

    EndPaint();

    Refresh();
}


void PanelGraph::BeginPaint()
{
    dc.SelectObject(bitmap);
    gc = wxGraphicsContext::Create(dc);
}


void PanelGraph::EndPaint()
{
    dc.SelectObject(wxNullBitmap);
}


void PanelGraph::FillRectangle(int x, int y, int width, int height, const wxColor &_color)
{
    color = _color;
    gc->SetBrush(color);
    gc->SetPen(color);
    gc->DrawRectangle( x, y, width, height );
}


void Point::Draw(int x, int y) const
{
    PanelGraph::self->gc->StrokeLine(x, y, x + 0.01, y);
}


void PanelGraph::DrawLine(int x1, int y1, int x2, int y2)
{
    gc->StrokeLine(x1, y1, x2, y2);
}


void PanelGraph::DrawLine(int x1, int y1, int x2, int y2, const wxColor &_color)
{
    color = _color;
    gc->SetPen(color);
    gc->StrokeLine(x1, y1, x2, y2);
}


Text::Text(const wxString &_text) : text(_text)
{

}


void Text::SetFont()
{
    PanelGraph::self->gc->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL), PanelGraph::self->color);
}


void Text::Draw(int x, int y) const
{
    PanelGraph::self->gc->DrawText(text, x, y);
}


void Text::DrawAboutCenterLeft(int x, int y) const
{
    double width, height, descent, externalLeading;
    PanelGraph::self->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    x -= (int)(width + 0.5);
    y -= (int)(height / 2.0 + 0.5);

    PanelGraph::self->gc->DrawText(text, x, y);
}


void Text::DrawAboutCenterDown(int x, int y) const
{
    double width, height, descent, externalLeading;
    PanelGraph::self->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    x -= (int)(width / 2.0 + 0.5);

    PanelGraph::self->gc->DrawText(text, x, y);
}