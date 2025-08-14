// 2025/08/13 14:19:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Slider.h"


SliderInt::SliderInt(wxWindow *parent, const wxPoint &position, int width, int _min, int _max) :
    wxPanel(parent, wxID_ANY, position, { width, TEXTCNTRL_HEIGHT + 5 + 5 }),
    min(_min),
    max(_max)
{
    int w1 = 20;
    int w2 = 17;

    slider = new wxSlider(this, wxID_ANY, min, min, max, { w1, 0 }, { width - w1 - w2, TEXTCNTRL_HEIGHT + 5 } );

    text = new wxStaticText(this, wxID_ANY, "0", { 0, 5 }, { w1, TEXTCNTRL_HEIGHT });

    wxSize size_button{ 15, 12 };

    int x = w1 + slider->GetSize().x;

    btnMore = new wxButton(this, wxID_ANY, "", { x, 0 }, size_button);
    btnLess = new wxButton(this, wxID_ANY, "", { x, size_button.y }, size_button);

    Bind(wxEVT_SLIDER, &SliderInt::OnEventSlider, this);
    btnMore->Bind(wxEVT_LEFT_DOWN, &SliderInt::OnEventMouseDown, this);
    btnLess->Bind(wxEVT_LEFT_DOWN, &SliderInt::OnEventMouseDown, this);
    btnMore->Bind(wxEVT_LEFT_UP, &SliderInt::OnEventMouseUp, this);
    btnLess->Bind(wxEVT_LEFT_UP, &SliderInt::OnEventMouseUp, this);

    timer_less.Bind(wxEVT_TIMER, &SliderInt::OnEventTimer, this);
    timer_more.Bind(wxEVT_TIMER, &SliderInt::OnEventTimer, this);
}


void SliderInt::OnEventSlider(wxCommandEvent &event)
{
    if (event.GetId() == slider->GetId())
    {
        text->SetLabel(wxString::Format("%d", event.GetInt()));
    }

    event.Skip();
}


void SliderInt::OnEventMouseDown(wxMouseEvent &event)
{
    if (event.GetId() == btnMore->GetId())
    {
        slider->SetValue(slider->GetValue() + 1);
        timer_more.Start(400);
    }
    else if (event.GetId() == btnLess->GetId())
    {
        slider->SetValue(slider->GetValue() - 1);
        timer_less.Start(400);
    }

    wxCommandEvent evt(wxEVT_SLIDER, slider->GetId());
    evt.SetEventObject(slider);
    evt.SetInt(slider->GetValue());
    wxPostEvent(slider, evt);

    event.Skip();
}


void SliderInt::OnEventMouseUp(wxMouseEvent &event)
{
    timer_more.Stop();
    timer_less.Stop();

    event.Skip();
}


void SliderInt::OnEventTimer(wxTimerEvent &event)
{
    int delta = (event.GetId() == timer_more.GetId()) ? 1 : -1;

    slider->SetValue(slider->GetValue() + delta);
    wxCommandEvent evt(wxEVT_SLIDER, slider->GetId());
    evt.SetEventObject(slider);
    evt.SetInt(slider->GetValue());
    wxPostEvent(slider, evt);

    event.Skip();
}


SliderFloat::SliderFloat(wxWindow *parent, const wxPoint &position, int width, double _min, double _max) :
    wxPanel(parent, wxID_ANY, position, { width, TEXTCNTRL_HEIGHT + 5 + 5 }),
    min(_min),
    max(_max)
{
    int w1 = 30;
    int w2 = 17;

    slider = new wxSlider(this, wxID_ANY, num_steps / 2, 0, num_steps, { w1, 0 }, { width - w1 - w2, TEXTCNTRL_HEIGHT + 5 });

    text = new wxStaticText(this, wxID_ANY, "0", { 0, 5 }, { w1, TEXTCNTRL_HEIGHT });

    SetRange(min, max);

    wxSize size_button{ 15, 12 };

    int x = w1 + slider->GetSize().x;

    btnMore = new wxButton(this, wxID_ANY, "", { x, 0 }, size_button);
    btnLess = new wxButton(this, wxID_ANY, "", { x, size_button.y }, size_button);

    Bind(wxEVT_SLIDER, &SliderFloat::OnEventSlider, this);
    btnMore->Bind(wxEVT_LEFT_DOWN, &SliderFloat::OnEventMouseDown, this);
    btnLess->Bind(wxEVT_LEFT_DOWN, &SliderFloat::OnEventMouseDown, this);
    btnMore->Bind(wxEVT_LEFT_UP, &SliderFloat::OnEventMouseUp, this);
    btnLess->Bind(wxEVT_LEFT_UP, &SliderFloat::OnEventMouseUp, this);

    timer_less.Bind(wxEVT_TIMER, &SliderFloat::OnEventTimer, this);
    timer_more.Bind(wxEVT_TIMER, &SliderFloat::OnEventTimer, this);
}


void SliderFloat::SetRange(double _min, double _max)
{
    min = _min;
    max = _max;

    step = (max - min) / num_steps;

    CalculateValue();
}


void SliderFloat::CalculateValue()
{
    double value = (max - min) * slider->GetValue() / num_steps;

    text->SetLabel(wxString::Format("%.2f", value));
}


void SliderFloat::OnEventSlider(wxCommandEvent &event)
{
    CalculateValue();

    event.Skip();
}


void SliderFloat::OnEventMouseDown(wxMouseEvent &event)
{

    if (event.GetId() == btnMore->GetId())
    {
        slider->SetValue(slider->GetValue() + 1);
        timer_more.Start(400);
    }
    else if (event.GetId() == btnLess->GetId())
    {
        slider->SetValue(slider->GetValue() - 1);
        timer_less.Start(400);
    }

    wxCommandEvent evt(wxEVT_SLIDER, slider->GetId());
    evt.SetEventObject(slider);
    evt.SetInt(slider->GetValue());
    wxPostEvent(slider, evt);

    event.Skip();
}


void SliderFloat::OnEventMouseUp(wxMouseEvent &event)
{
    timer_more.Stop();
    timer_less.Stop();

    event.Skip();
}


void SliderFloat::OnEventTimer(wxTimerEvent &event)
{
    int delta = (event.GetId() == timer_more.GetId()) ? 1 : -1;

    slider->SetValue(slider->GetValue() + delta);
    wxCommandEvent evt(wxEVT_SLIDER, slider->GetId());
    evt.SetEventObject(slider);
    evt.SetInt(slider->GetValue());
    wxPostEvent(slider, evt);

    event.Skip();
}
