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

    CreateMainPanel();

    Move(SET::GUI::calculation_pos.Get());

    Layout();
    Fit();
}


void DraggedWindow::CreateMainPanel()
{
    main_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxEXPAND | wxSTAY_ON_TOP);

#ifdef WIN32
    SetupDragging(main_panel);
#else
    SetupDragging(main_panel);
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


DraggedDialog::DraggedDialog(wxFrame *parent) : DraggedWindow(parent)
{
    SetWindowStyleFlag(wxFRAME_FLOAT_ON_PARENT | wxBORDER_SIMPLE);

    m_parent = parent;
    m_modalActive = false;
    m_modalResult = wxID_CANCEL;

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // Добавьте ваши контролы здесь
    wxStaticText *text = new wxStaticText(main_panel, wxID_ANY,
        "Модальное окно", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    mainSizer->Add(text, 0, wxEXPAND | wxALL, 20);

    // Кнопки OK/Cancel
    wxBoxSizer *btnSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *okBtn = new wxButton(main_panel, wxID_OK, "OK");
    wxButton *cancelBtn = new wxButton(main_panel, wxID_CANCEL, "Отмена");

    btnSizer->Add(okBtn, 0, wxALL, 5);
    btnSizer->Add(cancelBtn, 0, wxALL, 5);
    mainSizer->Add(btnSizer, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    main_panel->SetSizer(mainSizer);

    // Bind обработчики
    okBtn->Bind(wxEVT_BUTTON, &DraggedDialog::OnOK, this);
    cancelBtn->Bind(wxEVT_BUTTON, &DraggedDialog::OnCancel, this);
    Bind(wxEVT_CLOSE_WINDOW, &DraggedDialog::OnClose, this);

    Layout();
    Fit();
}


int DraggedDialog::ShowModal()
{
    m_modalActive = true;
    m_modalResult = wxID_CANCEL;

    // Блокируем родителя
    if (m_parent)
    {
        m_parent->Disable();
    }

    // Показываем окно с XFCE фиксом
    ShowWithXFCEFix();

    // Простой цикл ожидания
    while (m_modalActive)
    {
        wxYield();  // Обрабатываем события
        wxMilliSleep(10);  // Небольшая задержка чтобы не нагружать CPU
    }

    return m_modalResult;
}


void DraggedDialog::ShowWithXFCEFix()
{
    Show();
    Raise();
    SetFocus();

#ifdef __WXGTK__
    wxYield();  // Даем время GTK/XFCE

    GtkWidget *widget = GTK_WIDGET(GetHandle());
    if (widget && gtk_widget_get_window(widget))
    {
        GdkWindow *gdkWindow = gtk_widget_get_window(widget);
        gdk_window_set_keep_above(gdkWindow, TRUE);
        gdk_window_raise(gdkWindow);
    }
#endif
}


void DraggedDialog::OnClose(wxCloseEvent &/*event*/)
{
    CloseModal();
}


void DraggedDialog::OnOK(wxCommandEvent &/*event*/)
{
    m_modalResult = wxID_OK;
    CloseModal();
}


void DraggedDialog::OnCancel(wxCommandEvent &/*event*/)
{
    m_modalResult = wxID_CANCEL;
    CloseModal();
}


void DraggedDialog::CloseModal()
{
    m_modalActive = false;
    Hide();

    // Разблокируем родителя
    if (m_parent)
    {
        m_parent->Enable();
        m_parent->Raise();  // Поднимаем родительское окно
    }
}
