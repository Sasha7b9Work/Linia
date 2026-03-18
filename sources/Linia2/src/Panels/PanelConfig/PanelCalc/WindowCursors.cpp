// 2026/3/16 21:44:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelConfig/PanelCalc/WindowCursors.h"
#include "Settings/Settings.h"
#include "Display/Display.h"
#include "Utils/SystemDepend.h"


WindowCursors *TheWindowCursors = nullptr;


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
        chbTrackX = new ValueCheckBox(boxAuto, "Отслеживать по X", { x, SD::Y_SB(y) }, "track_x", false);

        y += dy;

        chbTrackY = new ValueCheckBox(boxAuto, "Отслеживать по Y", { x, SD::Y_SB(y) }, "track_y", false);

        y += dy;

        chbTrackMouse = new ValueCheckBox(boxAuto, "Указатель мыши", { x, SD::Y_SB(y) }, "track_mouse", false);
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

    event.Skip();
}
