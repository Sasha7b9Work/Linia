// 2026/3/16 21:44:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelConfig/PanelCalc/WindowCursors.h"
#include "Settings/Settings.h"
#include "Display/Display.h"
#include "Utils/SystemDepend.h"


WindowCursors::WindowCursors() : DraggedDialog("Курсоры", {200, 300})
{
    SetPosition(SET::GUI::cursors_pos.Get());

    int x = 10;
    int y = 22;
    int dy = 25;

    wxWindow *wnd = MainWidget();

    wxSize size = wnd->GetClientSize();

    const int d = 5;

    size.x -= 5 + 5;
    size.y = 100;

    wxStaticBox *boxAuto = new wxStaticBox(wnd, wxID_ANY, "Автоматические", { d, TopY() }, size);

    {
        chbTrackX = new wxCheckBox(boxAuto, wxID_ANY, "Отслеживать по X", { x, SD::Y_SB(y) });
        chbTrackX->SetValue(SET::GUI::track_x);

        y += dy;

        chbTrackY = new wxCheckBox(boxAuto, wxID_ANY, "Отслеживать по Y", { x, SD::Y_SB(y) });
        chbTrackY->SetValue(SET::GUI::track_y);

        y += dy;

        chbTrackMouse = new wxCheckBox(boxAuto, wxID_ANY, "Указатель мыши", { x, SD::Y_SB(y) });
        chbTrackMouse->SetValue(SET::GUI::track_mouse);
    }

    wxStaticBox *boxManual = new wxStaticBox(wnd, wxID_ANY, "Управляемые", { d, boxAuto->GetPosition().y + boxAuto->GetSize().y + d }, size);

    y = 22;

    {
        chbCursorsX = new wxCheckBox(boxManual, wxID_ANY, "X", { x, SD::Y_SB(y) });
        chbCursorsX->SetValue(SET::GUI::cursors_x);

        y += dy;

        chbCursorsY = new wxCheckBox(boxManual, wxID_ANY, "Y", { x, SD::Y_SB(y) });
        chbCursorsY->SetValue(SET::GUI::cursors_y);
    }

    Bind(wxEVT_CHECKBOX, &WindowCursors::OnEventCheckBox, this);
    Bind(wxEVT_CLOSE_WINDOW, &WindowCursors::OnEventClose, this);
}


WindowCursors::~WindowCursors()
{
    SET::GUI::cursors_pos.Set(GetPosition());
}


void WindowCursors::OnEventCheckBox(wxCommandEvent &event)
{
    int id = event.GetId();
    bool check = event.IsChecked();

    if (id == chbTrackX->GetId())
    {
        SET::GUI::track_x.Set(check);
        TheDisplay->Refresh();
    }
    else if (id == chbTrackY->GetId())
    {
        SET::GUI::track_y.Set(check);
        TheDisplay->Refresh();
    }
    else if (id == chbTrackMouse->GetId())
    {
        SET::GUI::track_mouse.Set(check);
        TheDisplay->Refresh();
    }
    else if (id == chbCursorsX->GetId())
    {
        SET::GUI::cursors_x.Set(check);
        TheDisplay->Refresh();
    }
    else if (id == chbCursorsY->GetId())
    {
        SET::GUI::cursors_y.Set(check);
        TheDisplay->Refresh();
    }
}


void WindowCursors::OnEventClose(wxCloseEvent &event)
{
    SET::GUI::cursors_pos.Set(GetPosition());
    event.Skip();
}
