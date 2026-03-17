// 2026/3/16 21:09:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Окно, которое можно передвигать мышью
class DraggedWindow : public wxFrame
{
public:

    DraggedWindow(const wxString &, const wxSize &);

    virtual bool Show(bool = true) override;

    void SetSize(const wxSize &);

protected:

    wxPanel *main_panel = nullptr;

private:

    wxPanel *titleBar = nullptr;
    bool     dragging = false;
    wxPoint  dragStart;
    wxString title;
    wxRect   closeButtonRect;       // Область кнопки закрытия
    bool     mouseInCloseButton;    // Для ховер-эффекта
    const int titleHeight = 30;

    void CreateTitleBar();

    void SetupDragging(wxWindow *);

    void OnMouseLeftDown(wxMouseEvent &);
    void OnDragEnd(wxMouseEvent &);
    void OnDragMotion(wxMouseEvent &);
    void OnPaint(wxPaintEvent &);
    void OnCloseEvent(wxCloseEvent &);
};


// В отличие от DraggedWindow, является модальным, т.е. все окна кроме этого неактивны
class DraggedDialog : public DraggedWindow
{
public:

    DraggedDialog(const wxString &, const wxSize &);

    int ShowModal();

private:

    virtual bool Show(bool = true) override;

    bool m_modalActive;
    int m_modalResult;
    wxWindow *m_parent;

    void ShowWithXFCEFix();

    void OnCloseEvent(wxCloseEvent &);

    void OnOK(wxCommandEvent &);

    void OnCancel(wxCommandEvent &);

    void CloseModal();
};
