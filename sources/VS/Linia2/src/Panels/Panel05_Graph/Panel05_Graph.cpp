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

    CreateEntities();

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

    grid.Draw(entities);

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


void PanelGraph::FillRectangle(int x, int y, int width, int height, const wxColor &_color)
{
    color = _color;
    gc->SetBrush(color);
    gc->SetPen(color);
    gc->DrawRectangle(x, y, width, height);
}


void Text::DrawAboutCenterDown(int x, int y, bool fillBackground, const wxColor &background) const
{
    double width, height, descent, externalLeading;
    PanelGraph::self->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    x -= (int)(width / 2.0 + 0.5);

    if (fillBackground)
    {
        PanelGraph::self->gc->SetBrush(background);
        PanelGraph::self->gc->SetPen(background);
        PanelGraph::self->gc->DrawRectangle(x, y, width, height);

        PanelGraph::self->gc->SetBrush(PanelGraph::self->color);
        PanelGraph::self->gc->SetPen(PanelGraph::self->color);
    }

    PanelGraph::self->gc->DrawText(text, x, y);
}


void Text::DrawAboutCenterRigth(int x, int y, bool fillBackground, const wxColor &background) const
{
    double width, height, descent, externalLeading;
    PanelGraph::self->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    y -= (int)(height / 2.0 + 0.5);

    if (fillBackground)
    {
        PanelGraph::self->gc->SetBrush(background);
        PanelGraph::self->gc->SetPen(background);
        PanelGraph::self->gc->DrawRectangle(x, y, width, height);

        PanelGraph::self->gc->SetBrush(PanelGraph::self->color);
        PanelGraph::self->gc->SetPen(PanelGraph::self->color);
    }

    PanelGraph::self->gc->DrawText(text, x, y);
}


void PanelGraph::CreateEntities()
{
    entities.push_back(new GraphLine(-1.0, -1.0, 1.0, 1.0, *wxBLUE));

    GraphMeasures *meas = new GraphMeasures(*wxGREEN);

    meas->AppendPoint({ 0.0, 0.0 });
    meas->AppendPoint({ 0.3, 0.5 });
    meas->AppendPoint({ 0.7, 1.0 });
    meas->AppendPoint({ 1.1, 1.5 });
    meas->AppendPoint({ 1.6, 2.0 });
    meas->AppendPoint({ 2.0, 2.5 });
    meas->AppendPoint({ 2.5, 3.0 });
    meas->AppendPoint({ 3.0, 3.4 });
    meas->AppendPoint({ 4.0, 4.0 });
    meas->AppendPoint({ 6.0, 4.3 });

    entities.push_back(meas);
}


void Spline::AppendPoint(const wxPoint2DDouble &point)
{
    points.push_back(point);
}


void Spline::Draw(bool smooth, bool draw_points) const
{
    wxGraphicsPath path = PanelGraph::self->gc->CreatePath();

    path.MoveToPoint(points[0].m_x, points[0].m_y);

    if (smooth)
    {
        for (uint i = 1; i < points.size(); i += 3)
        {
            path.AddCurveToPoint(
                points[i].m_x, points[i].m_y,
                points[i + 1].m_x, points[i + 1].m_y,
                points[i + 2].m_x, points[i + 2].m_y
            );
        }
    }
    else
    {
        for (uint i = 1; i < points.size(); i++)
        {
            path.AddLineToPoint(points[i].m_x, points[i].m_y);
        }
    }

    PanelGraph::self->gc->StrokePath(path);

    if (draw_points)
    {
        wxGraphicsPath path_circle = PanelGraph::self->gc->CreatePath();

        PanelGraph::self->gc->SetBrush(*wxRED_BRUSH);

        for (const auto &pt : points)
        {
            path_circle.AddCircle(pt.m_x, pt.m_y, 3);
        }

        PanelGraph::self->gc->FillPath(path_circle);
    }
}
