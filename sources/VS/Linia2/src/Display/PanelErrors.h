// 2025/09/01 23:00:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class PanelErrors : public wxPanel
{
public:

    PanelErrors(wxWindow *);

private:

    wxTextCtrl *text_ctrl = nullptr;
};
