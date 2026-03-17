// 2026/3/16 21:44:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelConfig/PanelCalc/WindowCursors.h"
#include "Settings/Settings.h"
#include "Display/Display.h"


WindowCursors::WindowCursors(const wxString &_title) : DraggedDialog(_title, { 200, 300 })
{
    SetPosition(SET::GUI::cursors_pos.Get());

    int x = 10;
    int y = 20;
    int dy = 25;

    wxWindow *wnd = MainWidget();

    wxSize size = wnd->GetClientSize();

    const int d = 5;

    size.x -= 5 + 5;
    size.y = 90;

    wxStaticBox *boxAuto = new wxStaticBox(wnd, wxID_ANY, "Автоматические", { d, TopY() }, size);

    {
        chbTrackX = new wxCheckBox(boxAuto, wxID_ANY, "Отслеживать по X", { x, y });
        chbTrackX->SetValue(SET::GUI::track_x);

        y += dy;

        chbTrackY = new wxCheckBox(boxAuto, wxID_ANY, "Отслеживать по Y", { x, y });
        chbTrackY->SetValue(SET::GUI::track_y);

        y += dy;

        chbTrackMouse = new wxCheckBox(boxAuto, wxID_ANY, "Указатель мыши", { x, y });
        chbTrackMouse->SetValue(SET::GUI::track_mouse);
    }

    wxStaticBox *boxManual = new wxStaticBox(wnd, wxID_ANY, "Управляемые", { d, boxAuto->GetPosition().y + boxAuto->GetSize().y + d }, size);
    (void)boxManual;

    Bind(wxEVT_CHECKBOX, &WindowCursors::OnEventCheckBox, this);
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
}
