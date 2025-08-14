// 2025/08/13 14:18:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class SliderInt : public wxPanel
{
public:

    SliderInt(wxWindow *parent, const wxPoint &position, int width, int min, int max);

private:

    int min = 0;
    int max = 0;

    wxStaticText *text = nullptr;
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

    SliderFloat(wxWindow *parent, const wxPoint &position, int width);

    void SetRange(double min, double max, const wxString &units, int digits_after_point);

    // Рассчитывает и устанавливает значения для данного range
    void CalculateAndSetRangeForRange(const wxString &range, double multiplier);

private:

    wxStaticText *text = nullptr;
    wxSlider     *slider = nullptr;
    wxButton     *btnMore = nullptr;
    wxButton     *btnLess = nullptr;

    wxTimer timer_less;
    wxTimer timer_more;

    int digitts_after_points = 0;
    wxString units;
    double min = 0.0;
    double max = 0.0;
    double step = 0.0f;
    const int num_steps = 100;

    void OnEventSlider(wxCommandEvent &);
    void OnEventMouseDown(wxMouseEvent &);
    void OnEventMouseUp(wxMouseEvent &);
    void OnEventTimer(wxTimerEvent &);

    // Рассчитать и показать новое значение
    void CalculateValue();
};
