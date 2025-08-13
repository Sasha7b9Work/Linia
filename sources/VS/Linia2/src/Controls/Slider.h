// 2025/08/13 14:18:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Slider : public wxPanel
{
public:

    Slider(wxWindow *parent, const wxPoint &positioni, int width);

private:

    wxStaticText *textValue = nullptr;
    wxSlider *slider = nullptr;
    wxButton *btnMore = nullptr;
    wxButton *btnLess = nullptr;

    wxTimer timer_less;
    wxTimer timer_more;

    void OnEventSlider(wxCommandEvent &);
    void OnEventMouseDown(wxMouseEvent &);
    void OnEventMouseUp(wxMouseEvent &);
    void OnEventTimer(wxTimerEvent &);
};
