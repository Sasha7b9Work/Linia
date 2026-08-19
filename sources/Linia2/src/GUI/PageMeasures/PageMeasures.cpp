// 2022/10/28 23:17:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "GUI/PageMeasures/PageMeasures.h"
#include "MainWindow.h"
#include "MainWindow.h"
#include "GUI/PageMeasures/Graphics/GraphMeasure.h"
#include "GUI/PageMeasures/MenuDisplay.h"
#include "GUI/PageMeasures/Graphics/AutoCursors.h"
#include "GUI/PageMeasures/PanelErrors.h"
#include "GUI/PageMeasures/Graphics/Splines.h"
#include "GUI/PageMeasures/Grid/Grid.h"
#include "Settings/Settings.h"
#include "Utils/Timer.h"
#pragma warning(push, 0)
#include <wx/msgdlg.h>
#include <wx/dcclient.h>
#pragma warning(pop)


PageMeasures *PageMeasures::self = nullptr;


PageMeasures::PageMeasures(Notebook *board) : PageNotebook(board, L("Измерения"))
{
    self = this;

    wxPanel::SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &PageMeasures::OnEventPaint, this);
    Bind(wxEVT_LEFT_DOWN, &PageMeasures::OnEventMouseDown, this);
    Bind(wxEVT_LEFT_UP, &PageMeasures::OnEventMouseUp, this);
    Bind(wxEVT_MOTION, &PageMeasures::OnEventMouseMove, this);
    Bind(wxEVT_MOUSEWHEEL, &PageMeasures::OnEventMouseWheel, this);
    Bind(wxEVT_RIGHT_DOWN, &PageMeasures::OnEventRightClick, this);
    Bind(wxEVT_LEAVE_WINDOW, &PageMeasures::OnEventLeaveWindow, this);
    Bind(wxEVT_ENTER_WINDOW, &PageMeasures::OnEventEnterWindow, this);
    Bind(wxEVT_BUTTON, &PageMeasures::OnEventButton, this);

    int w = 25;

    wxSize size{ w, w };

#define CREATE_BUTTON(btn, txt)          \
    btn = new Button(this, txt, size);   \
    buttons.push_back(btn);

    CREATE_BUTTON(btnHelp, "?")
    CREATE_BUTTON(btnLessX, "X-");
    CREATE_BUTTON(btnMoreX, "X+");
    CREATE_BUTTON(btnLessY, "Y-");
    CREATE_BUTTON(btnMoreY, "Y+");

    panel_errors = new PanelErrors(this);

    GraphMeasure::CreateForEmulator(entities);

    Init();
}


PageMeasures::~PageMeasures()
{
    SAFE_DELETE(bitmap);
    SAFE_DELETE(TheGrid);
    SAFE_DELETE(panel_errors);
}


void PageMeasures::FullScreen(bool full)
{
    if (!IsShown())
    {
        return;
    }

    full_screen = full;
}


void PageMeasures::Init()
{
    int width = full_screen ? MainWindow::WIDTH : MainWindow::WIDTH_DRAW;
    int height = full_screen ? MainWindow::HEIGHT : MainWindow::HEIGHT_DRAW;

    Panel::SetSize({ width, height });

    Panel::SetPosition({ 0, full_screen ? 0 : MainWindow::HEIGHT_HI });

    SAFE_DELETE(bitmap);

    bitmap = new wxBitmap(Panel::GetSize().x, Panel::GetSize().y);

    IGrid::Create(TheGrid);

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

    GraphMeasure::CreateForEmulator(entities);

    Refresh();
}


void PageMeasures::OnEventMouseDown(wxMouseEvent &event)
{
    pos_mouse_down = event.GetPosition();

    mouse_is_pressed = true;

    SetCursor(wxCursor(wxCURSOR_HAND));

    TheGrid->OnMouseDown();

    Refresh();
}


void PageMeasures::OnEventLeaveWindow(wxMouseEvent &event)
{
    if (mouse_is_pressed)
    {
        mouse_is_pressed = false;

        wxMouseEvent upEvent(wxEVT_LEFT_UP);
        upEvent.SetPosition(event.GetPosition());
        upEvent.SetEventObject(this);

        GetEventHandler()->ProcessEvent(upEvent);
    }

    TheAutoCursors->Ban();

    event.Skip();
}


void PageMeasures::OnEventEnterWindow(wxMouseEvent &event)
{
    TheAutoCursors->Allow();

    event.Skip();
}


void PageMeasures::OnEventMouseUp(wxMouseEvent &)
{
    mouse_is_pressed = false;

    SetCursor(wxCursor(wxCURSOR_ARROW));

    TheGrid->OnMouseUp();

    Refresh();
}


void PageMeasures::OnEventMouseMove(wxMouseEvent &event)
{
    wxPoint position = event.GetPosition();

    if (mouse_is_pressed)                            // Перемещение графика
    {
        wxPoint delta = position - pos_mouse_down;

        if (event.GetModifiers() == wxMOD_CONTROL)
        {
            TheGrid->MoveImageOn(delta);
        }
        else
        {
            TheGrid->MoveCenterOn(delta);
        }

        pos_mouse_down = position;

        if (!event.LeftIsDown())
        {

        }
    }
    else                                            // Отслеживание координат
    {
        TheGrid->SetNewMousePosition(position);
    }

    Refresh();
}


void PageMeasures::OnEventMouseWheel(wxMouseEvent &event)
{
    if (event.GetModifiers() == wxMOD_CONTROL)
    {
        TheGrid->ScaleGridOn(event.GetPosition(), event.GetWheelRotation());
    }
    else
    {
        TheGrid->RangeGridOnX(event.GetWheelRotation());
        TheGrid->RangeGridOnY(event.GetWheelRotation());
    }

    Refresh();
}


void PageMeasures::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnHelp->GetId())
    {
        wxMessageBox(_("Левая Кнопка Мыши - перемещение графика.\nКолёсико - масштаб графика.\n") +
            _("ЛКМ+Ctrl - перемещение сетки.\nКолёсико+Ctrl - масштаб сетки."), " ");
    }
    else if (id == btnLessX->GetId())
    {
        TheGrid->RangeGridOnX(-1);
    }
    else if (id == btnMoreX->GetId())
    {
        TheGrid->RangeGridOnX(+1);
    }
    else if (id == btnLessY->GetId())
    {
        TheGrid->RangeGridOnY(-1);
    }
    else if (id == btnMoreY->GetId())
    {
        TheGrid->RangeGridOnY(+1);
    }
}


void PageMeasures::BeginPaint()
{
    dc.SelectObject(*bitmap);
    gc = wxGraphicsContext::Create(dc);
    gc->SetAntialiasMode(wxANTIALIAS_NONE);
}


void PageMeasures::EndPaint()
{
    dc.SelectObject(wxNullBitmap);
}


void PageMeasures::OnEventPaint(wxPaintEvent &)
{
    if (!bitmap)
    {
        return;
    }

    BeginPaint();

    FillRectangle(0, 0, GetDrawingSize().x, GetDrawingSize().y, SET::GUI::color_background->Get());

    TheGrid->Draw(entities);

    EndPaint();

    wxPaintDC paint_dc(this);

    paint_dc.DrawBitmap(*bitmap, 0, 0);
}


void Point::Draw(int x, int y) const
{
    PageMeasures::self->gc->StrokeLine(x, y, x + 0.01, y);
}


void Line::Draw() const
{
    PageMeasures::self->gc->StrokeLine(x1, y1, x2, y2);
}


void Line::Draw(const wxColor &color) const
{
    PageMeasures::self->SetColorPen(color);
    PageMeasures::self->gc->StrokeLine(x1, y1, x2, y2);
}


void Rect::Fill(int x, int y, const wxColor &color) const
{
    PageMeasures::self->SetColorBrush(color);
    PageMeasures::self->gc->DrawRectangle(x, y, width, height);
}


void Rect::Draw(int x, int y, const wxColor &color) const
{
    PageMeasures::self->SetColorPen(color);
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
    PageMeasures::self->gc->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL), PageMeasures::self->color_pen);
}


void Text::Draw(int x, int y) const
{
    PageMeasures::self->gc->DrawText(text, x, y);
}


void Text::DrawAboutCenterLeft(int x, int y, bool fillBackground) const
{
    double width, height, descent, externalLeading;
    PageMeasures::self->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    x -= (int)(width + 0.5);
    y -= (int)(height / 2.0 + 0.5);

    if (fillBackground)
    {
        PageMeasures::self->gc->SetPen(PageMeasures::self->color_brush);
        PageMeasures::self->gc->DrawRectangle(x, y, width, height);
        PageMeasures::self->gc->SetPen(PageMeasures::self->color_pen);
    }

    PageMeasures::self->gc->DrawText(text, x, y);
}


void PageMeasures::FillRectangle(int x, int y, int width, int height, const wxColor &_color)
{
    SetColorBrush(_color);
    gc->DrawRectangle(x, y, width, height);
}


void Text::DrawAboutCenterDown(int x, int y, bool fillBackground) const
{
    double width, height, descent, externalLeading;
    PageMeasures::self->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    x -= (int)(width / 2.0 + 0.5);

    if (fillBackground)
    {
        PageMeasures::self->gc->SetPen(PageMeasures::self->color_brush);
        PageMeasures::self->gc->DrawRectangle(x, y, width, height);
        PageMeasures::self->gc->SetPen(PageMeasures::self->color_pen);
    }

    PageMeasures::self->gc->DrawText(text, x, y);
}


void Text::DrawAboutCenterUp(int x, int y, bool fillBackground) const
{
    double width, height, descent, externalLeading;
    PageMeasures::self->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    y -= (int)(height);
    x -= (int)(width / 2);

    if (fillBackground)
    {
        PageMeasures::self->gc->SetPen(PageMeasures::self->color_brush);
        PageMeasures::self->gc->DrawRectangle(x, y, width, height);
        PageMeasures::self->gc->SetPen(PageMeasures::self->color_pen);
    }

    PageMeasures::self->gc->DrawText(text, x, y);
}


void Text::DrawAboutRightUp(int x, int y, bool fillBackground, bool frame) const
{
    double width, height, descent, externalLeading;
    PageMeasures::self->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    y -= (int)(height);

    if (fillBackground)
    {
        PageMeasures::self->gc->SetPen(PageMeasures::self->color_brush);
        PageMeasures::self->gc->DrawRectangle(x, y, width, height);
        PageMeasures::self->gc->SetPen(PageMeasures::self->color_pen);

        if (frame)
        {
            Rect((int)width, (int)height).Draw(x, y, PageMeasures::self->color_pen);
        }
    }

    PageMeasures::self->gc->DrawText(text, x, y);
}


void Text::DrawAboutCenterRigth(int x, int y, bool fillBackground) const
{
    double width, height, descent, externalLeading;
    PageMeasures::self->gc->GetTextExtent(text, &width, &height, &descent, &externalLeading);

    y -= (int)(height / 2.0 + 0.5);

    if (fillBackground)
    {
        PageMeasures::self->gc->SetPen(PageMeasures::self->color_brush);
        PageMeasures::self->gc->DrawRectangle(x, y, width, height);
        PageMeasures::self->gc->SetPen(PageMeasures::self->color_pen);
    }

    PageMeasures::self->gc->DrawText(text, x, y);
}


void Spline::Draw(const std::vector<wxPoint> &points, bool smooth, bool draw_points) const
{
    if (smooth)
    {
        GraphicsSplineRenderer::DrawSplinePath(PageMeasures::self->gc, points, 1.0);
    }
    else
    {
        wxGraphicsPath path = PageMeasures::self->gc->CreatePath();

        path.MoveToPoint(points[0].x, points[0].y);

        for (uint i = 1; i < points.size(); i++)
        {
            path.AddLineToPoint(points[i].x, points[i].y);
        }

        PageMeasures::self->gc->StrokePath(path);
    }

    if (draw_points)
    {
        wxGraphicsPath path_circle = PageMeasures::self->gc->CreatePath();

        for (const auto &pt : points)
        {
            path_circle.AddCircle(pt.x, pt.y, SET::GUI::size_point->Get());
        }

        PageMeasures::self->gc->FillPath(path_circle);
    }
}


void PageMeasures::OnEventRightClick(wxMouseEvent &)
{
    MenuDisplay menu;

    PopupMenu(&menu);
}


void PageMeasures::SetColorBrush(const wxColor &_color)
{
    color_brush = _color;

    LoadColors();
}


void PageMeasures::SetColorPen(const wxColor &_color)
{
    color_pen = _color;

    LoadColors();
}


void PageMeasures::LoadColors()
{
    PageMeasures::self->gc->SetPen(color_pen);
    PageMeasures::self->gc->SetBrush(color_brush);
}


void PageMeasures::OnEventCnangeMeasuredElement()
{
}


wxSize PageMeasures::GetDrawingSize() const
{
    return wxPanel::GetClientSize();
}


wxSize PageMeasures::GetFullSize() const
{
    return wxPanel::GetSize();
}
