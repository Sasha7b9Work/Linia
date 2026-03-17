// 2026/3/16 21:43:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/DraggedWindow.h"


class WindowCursors : public DraggedDialog
{
public:

    WindowCursors(const wxString &);

private:

    wxCheckBox *chbTrackX = nullptr,
        *chbTrackY = nullptr,
        *chbTrackMouse = nullptr;

    virtual ~WindowCursors() override;

    void OnEventCheckBox(wxCommandEvent &);
};
