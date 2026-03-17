// 2026/3/16 21:09:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Окно, которое можно передвигать мышью
class DraggedWindow : public wxFrame
{
public:

    DraggedWindow(wxFrame *);

    virtual bool Show(bool = true) override;

protected:

    wxPanel *main_panel = nullptr;

private:

    void CreateMainPanel();

    void SetupDragging(wxWindow *);

    void OnDragStart(wxMouseEvent &);

    void OnDragEnd(wxMouseEvent &);

    void OnDragMotion(wxMouseEvent &);

    wxPanel *titleBar = nullptr;
    bool     dragging = false;
    wxPoint  dragStart;
};


// В отличие от DraggedWindow, является модальным, т.е. все окна кроме этого неактивны
class DraggedDialog : public DraggedWindow
{
public:

    DraggedDialog(wxFrame *);

    int ShowModal();

private:

    bool m_modalActive;
    int m_modalResult;
    wxWindow *m_parent;

    void ShowWithXFCEFix();

    void OnClose(wxCloseEvent &);

    void OnOK(wxCommandEvent &);

    void OnCancel(wxCommandEvent &);

    void CloseModal();
};
