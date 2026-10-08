// 2026/10/08 16:33:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#pragma warning(push, 0)
    #include <wx/popupwin.h>
#pragma warning(pop)


class PopupTransientWindow : public wxPopupTransientWindow
{
public:

    PopupTransientWindow(wxWindow *parent) :
        wxPopupTransientWindow(parent)
    {
        active_popup = this;
    }

    static PopupTransientWindow *GetActivePoup()
    {
        return active_popup;
    }

protected:

    virtual void OnDismiss() override
    {
        active_popup = nullptr;

        if (HasCapture())
        {
            ReleaseMouse();
        }

        wxPopupTransientWindow::OnDismiss();

        CallAfter([this]()
            {
                Destroy();
            });
    }

    static PopupTransientWindow *active_popup;
};
