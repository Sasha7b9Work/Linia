// 2025/08/13 14:18:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Controls.h"


class SliderInt : public wxPanel
{
public:

    SliderInt(wxWindow *parent, const wxPoint &position, int width, int min, int max, const wxString &name);

    void Pack();
    void Unpack();

    int GetValue() const;
    void SetValue(int);

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

    void Pack();
    void Unpack();

protected:

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
    virtual void CalculateValue();
};


class SliderFloatPercents : public SliderFloat
{
public:

    SliderFloatPercents(wxWindow *parent, const wxPoint &position, int width, int min_percents, int max_percents);

    virtual void CalculateValue() override;

protected:

    int min_percents = 0;
    int max_percents = 0;

private:

    wxStaticText *textPercents = nullptr;
};


class SliderFloatOffset : public SliderFloatPercents
{
public:

    SliderFloatOffset(wxWindow *parent, const wxPoint &position, int width) :
        SliderFloatPercents(parent, position, width, 0, 100) { }

    void CalculateAndSetRange(const wxString &range, double multiplier);
};


class SliderFloatLimit : public SliderFloatPercents
{
public:

    SliderFloatLimit(wxWindow *parent, const wxPoint &position, int width) :
        SliderFloatPercents(parent, position, width, 10, 110) { }

    void CalculateAndSetRange(const wxString &range);
};
