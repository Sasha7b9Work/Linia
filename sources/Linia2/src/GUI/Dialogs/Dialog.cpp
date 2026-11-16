// 2025/7/25 12:23:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/Dialogs/Dialog.h"
#include "Utils/SystemDepend.h"
#include "GUI/Controls/StaticText.h"
#pragma warning(push, 0)
    #include <wx/radiobut.h>
    #include <wx/statline.h>
#pragma warning(pop)


Dialog::Dialog(const wxString &_title, const wxPoint &_pos, const wxSize &_size) :
    DraggedDialog(_title, _size)
{
    wxFrame::SetPosition(_pos);
    Bind(wxEVT_CHAR_HOOK, &Dialog::OnKeyDown, this);
}


void Dialog::ShowOnWindow(wxWindow *window)
{
    wxSize size = window->GetSize();

    wxPoint pos = window->ClientToScreen(wxPoint(size.x / 2, size.y / 2));

    pos -= GetSize() / 2;

    Fit();
    Layout();

    if (GF::IsBoardOPi5Plus())
    {
    }
    else
    {
        int win_bottom = MainWindow::self->GetRect().GetBottom();
        int win_left = MainWindow::self->GetRect().GetLeft();

        int bottom = pos.y + GetSize().y;
        int left = pos.x;

        if (bottom >= win_bottom)
        {
            pos.y = win_bottom - GetSize().y;
        }
        if (left <= win_left)
        {
            pos.x = win_left;
        }
    }

    wxFrame::SetPosition(pos);

    DraggedDialog::ShowModal();
}


void Dialog::OnKeyDown(wxKeyEvent &event)
{
    if (event.GetKeyCode() == WXK_ESCAPE)
    {
        DraggedDialog::CloseModal();
    }
    else
    {
        event.Skip();
    }
}
