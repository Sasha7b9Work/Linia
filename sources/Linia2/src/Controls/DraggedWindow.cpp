// 2026/3/16 21:10:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/DraggedWindow.h"
#include "Settings/Settings.h"
#include "MainWindow.h"


DraggedWindow::DraggedWindow(wxFrame *parent)
    : wxFrame(parent, wxID_ANY, "WindowCalculation",
        wxPoint(100, 100), wxSize(450, 350),
        wxFRAME_FLOAT_ON_PARENT | wxBORDER_SIMPLE | wxSTAY_ON_TOP)
{
    // Отключаем стандартный заголовок
    SetWindowStyleFlag(wxFRAME_FLOAT_ON_PARENT | wxBORDER_SIMPLE);

    // Создаем основной цвет фона
    SetBackgroundColour(wxColour(240, 240, 245));

    CreateMainPanel();

    Move(SET::GUI::calculation_pos.Get());
}


void DraggedWindow::CreateMainPanel()
{
    wxPanel *panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxNO_BORDER | wxEXPAND | wxSTAY_ON_TOP);
    (void)panel;

#ifdef WIN32
    SetupDragging(this);
#else
    SetupDragging(panel);
#endif
}


void DraggedWindow::SetupDragging(wxWindow *window)
{
    window->Bind(wxEVT_LEFT_DOWN, &DraggedWindow::OnDragStart, this);
    window->Bind(wxEVT_LEFT_UP, &DraggedWindow::OnDragEnd, this);
    window->Bind(wxEVT_MOTION, &DraggedWindow::OnDragMotion, this);
}

void DraggedWindow::OnDragStart(wxMouseEvent &event)
{
    if (!dragging)
    {
        wxWindow *source = (wxWindow *)event.GetEventObject();
        if (source && !source->HasCapture())
        {
            source->CaptureMouse();
        }

        dragging = true;
        dragStart = wxGetMousePosition();

    }

    event.Skip();
}


void DraggedWindow::OnDragEnd(wxMouseEvent &event)
{
    if (dragging)
    {
        wxWindow *source = (wxWindow *)event.GetEventObject();
        if (source && source->HasCapture())
        {
            source->ReleaseMouse();
        }

        dragging = false;
    }

    event.Skip();
}


void DraggedWindow::OnDragMotion(wxMouseEvent &event)
{
    wxWindow *source = (wxWindow *)event.GetEventObject();

    if (dragging && event.Dragging() && source && source->HasCapture())
    {
        wxPoint currentPos = wxGetMousePosition();
        wxPoint delta = currentPos - dragStart;
        wxPoint newPos = GetPosition() + delta;
        Move(newPos);
        SET::GUI::calculation_pos.Set(newPos);
        dragStart = currentPos;
    }

    event.Skip();
}


bool DraggedWindow::Show(bool show)
{
    bool result = wxFrame::Show(show);

    MainWindow::self->HideSystemPanel();

    return result;
}
