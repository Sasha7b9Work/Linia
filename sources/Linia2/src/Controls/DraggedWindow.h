// 2026/3/16 21:09:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Окно, которое можно передвигать мышью


class DraggedWindow : public wxFrame
{
public:

    DraggedWindow(wxFrame *);

private:

    void CreateTitleBar(wxWindow *, wxBoxSizer *);

    void CreateMainPanel();

    void CreateMainPanel2();

    void SetupDragging(wxWindow *);

    void OnDragStart(wxMouseEvent &);

    void OnDragEnd(wxMouseEvent &);

    void OnDragMotion(wxMouseEvent &);

    wxPanel *titleBar = nullptr;
    bool     dragging = false;
    wxPoint  dragStart;
};
