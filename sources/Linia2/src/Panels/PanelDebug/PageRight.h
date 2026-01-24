// 2026/01/24 21:41:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class PanelRight : public wxPanel
{
public:

    PanelRight(wxWindow *);

    static PanelRight *self;

private:

    wxButton *btnReturn = nullptr;

    void OnEventButton(wxCommandEvent &);
};
