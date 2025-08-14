// 2025/08/13 14:18:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class SliderInt : public wxPanel
{
public:

    SliderInt(wxWindow *parent, const wxPoint &position, int width, int min, int max);

private:

    int min = 0;
    int max = 0;

    wxStaticText *textValue = nullptr;
    wxSlider     *slider = nullptr;
    wxButton     *btnMore = nullptr;
    wxButton     *btnLess = nullptr;

    wxTimer timer_less;
    wxTimer timer_more;

    void OnEventSlider(wxCommandEvent &);
    void OnEventMouseDown(wxMouseEvent &);
    void OnEventMouseUp(wxMouseEvent &);
    void OnEventTimer(wxTimerEvent &);
};


class SliderFloat : public wxPanel
{
public:

    SliderFloat(wxWindow *parent, const wxPoint &position, int width, double min, double max);

    void SetRange(float min, float max);

private:

    wxStaticText *textValue = nullptr;
    wxSlider     *slider = nullptr;
    wxButton     *btnMore = nullptr;
    wxButton     *btnLess = nullptr;

    wxTimer timer_less;
    wxTimer timer_more;

    double min = 0.0;
    double max = 0.0;

    void OnEventSlider(wxCommandEvent &);
    void OnEventMouseDown(wxMouseEvent &);
    void OnEventMouseUp(wxMouseEvent &);
    void OnEventTimer(wxTimerEvent &);
};
