// 2026/3/16 21:44:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelConfig/PanelCalc/WindowCursors.h"
#include "Settings/Settings.h"


WindowCursors::WindowCursors(const wxString &_title, const wxSize &_size) : DraggedDialog(_title, _size)
{
    SetPosition(SET::GUI::cursors_pos.Get());
}


WindowCursors::~WindowCursors()
{
    SET::GUI::cursors_pos.Set(GetPosition());
}
