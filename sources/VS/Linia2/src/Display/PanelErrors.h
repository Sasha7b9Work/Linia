// 2025/09/01 23:00:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Error
{
    enum E
    {
        _1 = 1,
        _2,
        _3,
        _4,
        Count
    };
};


class PanelErrors : public wxPanel
{
public:

    PanelErrors(wxWindow *);

    static PanelErrors *self;

    void ReInit();

    void AppendError(Error::E, const wxString &);
    void RemoveError(Error::E);

private:

    //    wxButton *btnCollapse = nullptr;

    wxTextCtrl *text_ctrl = nullptr;

    bool collapse = false;

    wxTimer timer;

    std::map<Error::E, wxString> errors;

    void OnEventButton(wxCommandEvent &);
    void OnEventTimer(wxTimerEvent &);

    void SetColors(bool inverse);

    void BuildCanvas();
};
