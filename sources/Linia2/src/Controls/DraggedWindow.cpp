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


DraggedDialog::DraggedDialog(wxFrame *parent) : DraggedWindow(parent)
{
    // Отключаем стандартный заголовок
    SetWindowStyleFlag(wxFRAME_FLOAT_ON_PARENT | wxBORDER_SIMPLE);

    // Создаем основной цвет фона
    SetBackgroundColour(wxColour(240, 240, 245));

    m_parent = parent;
    m_isModal = false;
    m_modalLoop = nullptr;

    // Bind events
    Bind(wxEVT_CLOSE_WINDOW, &DraggedDialog::OnClose, this);
    Bind(wxEVT_ACTIVATE, &DraggedDialog::OnActivate, this);

    // Для XFCE особенно важно
    Bind(wxEVT_SHOW, &DraggedDialog::OnShow, this);
}


int DraggedDialog::ShowModal()
{
    if (m_isModal)
        return wxID_CANCEL;

    m_isModal = true;

    // Блокируем родительское окно
    if (m_parent && m_parent->IsEnabled())
    {
        m_parent->Disable();
    }

    // Блокируем все остальные окна верхнего уровня
    wxWindowList &windows = wxTopLevelWindows;
    for (wxWindowList::iterator it = windows.begin(); it != windows.end(); ++it)
    {
        wxWindow *win = *it;
        if (win != this && win->IsEnabled())
        {
            win->Disable();
        }
    }

    // Показываем окно с специальной обработкой для XFCE
    ShowWindowWithXFCEFix();

    // СОЗДАЕМ КОНКРЕТНЫЙ КЛАСС, А НЕ АБСТРАКТНЫЙ
    wxGUIEventLoop loop;  // Создаем на стеке, не через new
    m_modalLoop = &loop;   // Сохраняем указатель

    int returnCode = loop.Run();  // Запускаем цикл

    m_modalLoop = nullptr;  // Обнуляем указатель после завершения

    return returnCode;
}


void DraggedDialog::EndModal(int retCode)
{
    if (!m_isModal)
        return;

    m_isModal = false;

    // Разблокируем окна
    if (m_parent)
    {
        m_parent->Enable();
    }

    wxWindowList &windows = wxTopLevelWindows;
    for (wxWindowList::iterator it = windows.begin(); it != windows.end(); ++it)
    {
        wxWindow *win = *it;
        if (win != this && !win->IsEnabled())
        {
            win->Enable();
        }
    }

    // Выходим из модального цикла
    if (m_modalLoop)
    {
        m_modalLoop->Exit(retCode);
    }

    // Скрываем окно
    Hide();
}


void DraggedDialog::ShowWindowWithXFCEFix()
{
    // Показываем окно
    Show();

    // Специальная обработка для XFCE
#ifdef __WXGTK__
        // Даем время XFCE на обработку
    wxYield();

    // Получаем X11 окно
    GdkWindow *gdkWindow = gtk_widget_get_window(GTK_WIDGET(GetHandle()));
    if (gdkWindow)
    {
        // Устанавливаем окно выше всех
        gdk_window_set_keep_above(gdkWindow, TRUE);

        // Поднимаем окно
        gdk_window_raise(gdkWindow);

        // Для XFCE также полезно установить тип окна
        gdk_window_set_type_hint(gdkWindow, GDK_WINDOW_TYPE_HINT_DIALOG);
    }

    // X11 прямой доступ
    Display *display = GDK_DISPLAY_XDISPLAY(gdk_display_get_default());
    Window xid = GDK_WINDOW_XID(gdkWindow);

    if (xid)
    {
        // Устанавливаем атрибут "выше всех"
        Atom wmState = XInternAtom(display, "_NET_WM_STATE", False);
        Atom wmAbove = XInternAtom(display, "_NET_WM_STATE_ABOVE", False);

        XChangeProperty(display, xid, wmState, XA_ATOM, 32,
            PropModeReplace, (unsigned char *)&wmAbove, 1);

        // Поднимаем окно
        XRaiseWindow(display, xid);
        XFlush(display);
    }
#endif

    // Устанавливаем фокус
    SetFocus();

    // Дополнительный подъем через wxWidgets
    Raise();
}


void DraggedDialog::OnShow(wxShowEvent &event)
{
    if (event.IsShown() && m_isModal)
    {
        // При показе модального окна, гарантируем что оно сверху
        CallAfter([this]()
            {
                ShowWindowWithXFCEFix();
            });
    }
    event.Skip();
}


void DraggedDialog::OnActivate(wxActivateEvent &event)
{
    if (m_isModal && !event.GetActive())
    {
        // Если модальное окно теряет активацию, возвращаем его сверху
        CallAfter([this]()
            {
                if (m_isModal && IsShown())
                {
                    ShowWindowWithXFCEFix();
                }
            });
    }
    event.Skip();
}


void DraggedDialog::OnClose(wxCloseEvent &event)
{
    if (m_isModal)
    {
        EndModal(wxID_CANCEL);
    }
    else
    {
        event.Skip();
    }
}
