// 2026/03/16 09:12:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class WindowCalculation : public wxFrame
{
public:
    WindowCalculation(wxFrame *);

private:

    void CreateTitleBar(wxWindow *, wxBoxSizer *);

    void CreateMainPanel();

    void CreateMainPanel2();

    void SetupDragging(wxWindow *);

    void OnDragStart(wxMouseEvent &);

    void OnDragEnd(wxMouseEvent &);

    void OnDragMotion(wxMouseEvent &);

private:
    wxPanel *titleBar = nullptr;
    bool     dragging = false;
    wxPoint  dragStart;
};
