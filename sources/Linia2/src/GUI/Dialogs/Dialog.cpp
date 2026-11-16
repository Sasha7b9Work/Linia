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
