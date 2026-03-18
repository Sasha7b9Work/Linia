// 2026/3/16 21:43:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/DraggedWindow.h"
#include "Settings/Value.h"


class WindowCursors : public DraggedDialog
{
public:

    WindowCursors();

    virtual ~WindowCursors() override;

    ValueCheckBox *chbTrackX = nullptr,
        *chbTrackY = nullptr;

private:

    wxCheckBox *chbTrackMouse = nullptr,
        *chbCursorsX = nullptr,
        *chbCursorsY = nullptr;

    void OnEventCheckBox(wxCommandEvent &);
};
