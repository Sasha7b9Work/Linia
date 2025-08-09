// 2025/8/9 11:41:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

//  нопка с рисунком, по нажатию на которую открываетс€ окно с другими выборами


class BmpButtonsCombo : public wxButton
{
public:

    BmpButtonsCombo(wxWindow *parent, const wxPoint &pos, const wxSize &, const wxString &file_bitmap);

private:

    wxBitmap bitmap;

    bool left_is_down = false;

    void OnEventPaint(wxPaintEvent &);
    void OnLeftDown(wxMouseEvent &);
    void OnLeftUp(wxMouseEvent &);
    void OnLeaveWindow(wxMouseEvent &);
};
