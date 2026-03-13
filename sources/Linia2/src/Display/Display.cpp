// 2022/10/28 23:17:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "MainWindow.h"
#include "MainWindow.h"
#include "Panels/PanelTable.h"
#include "Display/GraphEntity.h"
#include "Display/MenuDisplay.h"


Display *Display::self = nullptr;


Display::Display(wxWindow *parent) :
    Panel(parent, 0, MainWindow::HEIGHT_HI, MainWindow::WIDTH_DRAW, MainWindow::HEIGHT_DRAW)
{
    self = this;

    Panel::SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &Display::OnEventPaint, this);
    Bind(wxEVT_LEFT_DOWN, &Display::OnEventMouseDown, this);
    Bind(wxEVT_LEFT_UP, &Display::OnEventMouseUp, this);
    Bind(wxEVT_MOTION, &Display::OnEventMouseMove, this);
    Bind(wxEVT_MOUSEWHEEL, &Display::OnEventMouseWheel, this);
    Bind(wxEVT_RIGHT_DOWN, &Display::OnEventRightClick, this);
    Bind(wxEVT_BUTTON, &Display::OnEventButton, this);

    int w = 25;

    wxSize size{ w, w };

    btnHelp = new wxButton(this, wxID_ANY, "?", wxDefaultPosition, size);
    btnLessX = new wxButton(this, wxID_ANY, "X-", wxDefaultPosition, size);
    btnMoreX = new wxButton(this, wxID_ANY, "X+", wxDefaultPosition, size);
    btnLessY = new wxButton(this, wxID_ANY, "Y-", wxDefaultPosition, size);
    btnMoreY = new wxButton(this, wxID_ANY, "Y+", wxDefaultPosition, size);

    panel_errors = new PanelErrors(this);

    new PanelTable(this);

    PanelTable::self->Hide();

    Init();
}


Display::~Display()
{
    SAFE_DELETE(bitmap);
    SAFE_DELETE(IGrid::self);
    SAFE_DELETE(panel_errors);
}


void Display::FullScreen(bool full)
{
    if (!IsShown())
    {
        return;
    }

    full_screen = full;

    Init();
}


void Display::Init()
{
    int width = full_screen ? MainWindow::WIDTH : MainWindow::WIDTH_DRAW;
    int height = full_screen ? MainWindow::HEIGHT : MainWindow::HEIGHT_DRAW;

    Panel::SetSize({ width, height });

    Panel::SetPosition({ 0, full_screen ? 0 : MainWindow::HEIGHT_HI });

    SAFE_DELETE(bitmap);

    bitmap = new wxBitmap(Panel::GetSize().x, Panel::GetSize().y);

    IGrid::Create();

    int w = btnHelp->GetSize().x;
    int d = 10;
    int x0 = Panel::GetSize().x - d - btnHelp->GetSize().x;
    int y0 = d;

    btnHelp->SetPosition({ x0, y0 });

    btnLessX->SetPosition({ x0 - 2 * (w + d), y0 });
    btnMoreX->SetPosition({ x0 - 1 * (w + d), y0 });

    btnLessY->SetPosition({ x0, y0 + 2 * (w + d) });
    btnMoreY->SetPosition({ x0, y0 + w + d });

    panel_errors->ReInit();

    Panel::Layout();

    GraphEntity::CreateForEmulator(entities);

    Refresh();
}


void Display::OnEventMouseDown(wxMouseEvent &event)
{
    pos_mouse_down = event.GetPosition();

    mouse_is_pressed = true;

    SetCursor(wxCursor(wxCURSOR_HAND));

    IGrid::self->OnMouseDown();

    Refresh();
}


void Display::OnEventMouseUp(wxMouseEvent &)
{
    mouse_is_pressed = false;

    SetCursor(wxCursor(wxCURSOR_ARROW));

    IGrid::self->OnMouseUp();

    Refresh();
}


void Display::OnEventMouseMove(wxMouseEvent &event)
{
    wxPoint position = event.GetPosition();

    if (mouse_is_pressed)                            // Перемещение графика
    {
        wxPoint delta = position - pos_mouse_down;

        if (event.GetModifiers() == wxMOD_CONTROL)
        {
            IGrid::self->MoveImageOn(delta);
        }
        else
        {
            IGrid::self->MoveCenterOn(delta);
        }

        pos_mouse_down = position;
    }
    else                                            // Отслеживание координат
    {
        IGrid::self->SetNewMousePosition(position);
    }

    Refresh();
}


void Display::OnEventMouseWheel(wxMouseEvent &event)
{
    if (event.GetModifiers() == wxMOD_CONTROL)
    {
        IGrid::self->ScaleGridOn(event.GetPosition(), event.GetWheelRotation());
    }
    else
    {
        IGrid::self->RangeGridOnX(event.GetWheelRotation());
        IGrid::self->RangeGridOnY(event.GetWheelRotation());
    }

    Refresh();
}


void Display::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnHelp->GetId())
    {
        wxMessageBox("Левая Кнопка Мыши - перемещение графика.\nКолёсико - масштаб графика.\n"
            "ЛКМ+Ctrl - перемещение сетки.\nКолёсико+Ctrl - масштаб сетки.", " ");
    }
    else if (id == btnLessX->GetId())
    {
        IGrid::self->RangeGridOnX(-1);
    }
    else if (id == btnMoreX->GetId())
    {
        IGrid::self->RangeGridOnX(+1);
    }
    else if (id == btnLessY->GetId())
    {
        IGrid::self->RangeGridOnY(-1);
    }
    else if (id == btnMoreY->GetId())
    {
        IGrid::self->RangeGridOnY(+1);
    }
}


void Display::BeginPaint()
{
    dc.SelectObject(*bitmap);
    gc = wxGraphicsContext::Create(dc);
    gc->SetAntialiasMode(wxANTIALIAS_NONE);
}


void Display::EndPaint()
{
    dc.SelectObject(wxNullBitmap);
}


void Display::OnEventPaint(wxPaintEvent &)
{
    BeginPaint();

    FillRectangle(0, 0, GetDrawingSize().x, GetDrawingSize().y, *wxWHITE);

    IGrid::self->Draw(entities);

    EndPaint();

    wxPaintDC paint_dc(this);

    paint_dc.DrawBitmap(*bitmap, 0, 0);
}


void Point::Draw(int x, int y) const
{
    Display::self->gc->StrokeLine(x, y, x + 0.01, y);
}


void Line::Draw() const
{
    Display::self->gc->StrokeLine(x1, y1, x2, y2);
}


void Line::Draw(const wxColor &color) const
{
    Display::self->SetColor(color);
    Display::self->gc->StrokeLine(x1, y1, x2, y2);
}


void Rect::Fill(int x, int y, const wxColor &color) const
{
    Display::self->SetColor(color);
    Display::self->gc->DrawRectangle(x, y, width, height);
}


void Rect::Draw(int x, int y, const wxColor &color) const
{
    Display::self->SetColor(color);
    Line(x, y, x + width, y).Draw();
    Line(x + width, y, x + width, y + height).Draw();
    Line(x, y + height, x + width, y + height).Draw();
    Line(x, y, x, y + height).Draw();
}


Text::Text(const wxString &_text) : text(_text)
{

}


void Text::SetFont()
{
    Display::self->gc->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL), Display::self->color);
}


void Text::Draw(int x, int y) const
{
    Display::self->gc->DrawText(text, x, y);
}


void Text::DrawAboutCenterLeft(int x, int y, bool fillBackground, const wxColor &background) const
{
    double width, height, descent, externalLeading;
    Display::self->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    x -= (int)(width + 0.5);
    y -= (int)(height / 2.0 + 0.5);

    if (fillBackground)
    {
        Display::self->gc->SetBrush(background);
        Display::self->gc->SetPen(background);
        Display::self->gc->DrawRectangle(x, y, width, height);

        Display::self->gc->SetBrush(Display::self->color);
        Display::self->gc->SetPen(Display::self->color);
    }

    Display::self->gc->DrawText(text, x, y);
}


void Display::FillRectangle(int x, int y, int width, int height, const wxColor &_color)
{
    SetColor(_color);
    gc->DrawRectangle(x, y, width, height);
}


void Text::DrawAboutCenterDown(int x, int y, bool fillBackground, const wxColor &background) const
{
    double width, height, descent, externalLeading;
    Display::self->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    x -= (int)(width / 2.0 + 0.5);

    if (fillBackground)
    {
        Display::self->gc->SetBrush(background);
        Display::self->gc->SetPen(background);
        Display::self->gc->DrawRectangle(x, y, width, height);

        Display::self->gc->SetBrush(Display::self->color);
        Display::self->gc->SetPen(Display::self->color);
    }

    Display::self->gc->DrawText(text, x, y);
}


void Text::DrawAboutCenterUp(int x, int y, bool fillBackground, const wxColor &background, bool bound) const
{
    double width, height, descent, externalLeading;
    Display::self->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    y -= (int)(height);
    x -= (int)(width / 2);

    if (fillBackground)
    {
        Display::self->gc->SetBrush(background);
        Display::self->gc->SetPen(background);
        Display::self->gc->DrawRectangle(x, y, width, height);

        Display::self->LoadColor();
    }

    Display::self->gc->DrawText(text, x, y);

    if (bound)
    {
        Display::self->gc->SetBrush(*wxTRANSPARENT_BRUSH);

        Display::self->gc->DrawRectangle(x - 1, y - 1, width + 2, height + 2);

        Display::self->LoadColor();
    }
}


void Text::DrawAboutRightUp(int x, int y, bool fillBackground, const wxColor &background, bool bound) const
{
    double width, height, descent, externalLeading;
    Display::self->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    y -= (int)(height);

    if (fillBackground)
    {
        Display::self->gc->SetBrush(background);
        Display::self->gc->SetPen(background);
        Display::self->gc->DrawRectangle(x, y, width, height);

        Display::self->LoadColor();
    }

    Display::self->gc->DrawText(text, x, y);

    if (bound)
    {
        Display::self->gc->SetBrush(*wxTRANSPARENT_BRUSH);

        Display::self->gc->DrawRectangle(x - 1, y - 1, width + 2, height + 2);

        Display::self->LoadColor();
    }
}


void Text::DrawAboutCenterRigth(int x, int y, bool fillBackground, const wxColor &background) const
{
    double width, height, descent, externalLeading;
    Display::self->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    y -= (int)(height / 2.0 + 0.5);

    if (fillBackground)
    {
        Display::self->gc->SetBrush(background);
        Display::self->gc->SetPen(background);
        Display::self->gc->DrawRectangle(x, y, width, height);

        Display::self->LoadColor();
    }

    Display::self->gc->DrawText(text, x, y);
}


void Spline::AppendPoint(const wxPoint2DDouble &point)
{
    points.push_back(point);
}


void Spline::Draw(bool smooth, bool draw_points) const
{
    wxGraphicsPath path = Display::self->gc->CreatePath();

    Display::self->SetColor(Display::self->color);

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

    Display::self->gc->StrokePath(path);

    if (draw_points)
    {
        wxGraphicsPath path_circle = Display::self->gc->CreatePath();

        for (const auto &pt : points)
        {
            path_circle.AddCircle(pt.m_x, pt.m_y, 2);
        }

        Display::self->gc->FillPath(path_circle);
    }
}


void Display::OnEventRightClick(wxMouseEvent &)
{
    MenuDisplay menu;

    PopupMenu(&menu);
}


void Display::SetColor(const wxColor &_color)
{
    color = _color;

    LoadColor();
}


void Display::LoadColor()
{
    Display::self->gc->SetPen(color);
    Display::self->gc->SetBrush(color);
}


void Display::OnEventCnangeMeasuredElement()
{
}


wxSize Display::GetDrawingSize() const
{
    return Panel::GetClientSize();
}


wxSize Display::GetFullSize() const
{
    return Panel::GetSize();
}


