// 2023/09/02 11:37:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Buttons.h"
#include "Controls/Painter.h"
#include "Utils/GlobalFunctions.h"


ButtonColor::ButtonColor(wxWindow *parent, int id, const wxString &title, wxPoint position, wxSize size, Painter *painter) :
    wxButton(parent, id, title, position, size),
    m_painter(painter)
{
//    painter = new Painter(this, { 144, 5 }, { 16, 16 }, type);

    m_painter->Bind(wxEVT_LEFT_UP, &ButtonColor::OnMouseEvent, this);
}


void ButtonColor::SetColor(const Color &color)
{
    m_painter->SetColor(color);
}


void ButtonColor::OnMouseEvent(wxMouseEvent &event)
{
    if (event.IsButton())
    {
        GF::SendCommandEvent(GetParent(), wxEVT_BUTTON, GetId());
    }

    event.Skip();
}


CheckButton::CheckButton(wxWindow *parent, int id, const wxString &title, const wxPoint &pos, const wxSize &size) :
    wxPanel(parent, id, pos, size)
{
    button = new wxToggleButton(this, id + delta_id, title, { 0, 0 }, { size.x - 20, size.y });

    ch_box = new wxCheckBox(this, id + delta_id + 1, "", { size.x - 20 + 5, 4 });

    Bind(wxEVT_CHECKBOX, &CheckButton::OnEventCheckBox, this);
    Bind(wxEVT_TOGGLEBUTTON, &CheckButton::OnEventToggleButton, this);
}


void CheckButton::OnEventCheckBox(wxCommandEvent &event)
{
    if (event.GetId() == ch_box->GetId())
    {
        button->SetValue(event.GetInt() != 0);

        GF::SendCommandEvent(GetParent(), wxEVT_TOGGLEBUTTON, button->GetId(), button->GetValue() ? 1 : 0);

        event.Skip();
    }
}


void CheckButton::OnEventToggleButton(wxCommandEvent &event)
{
    if (event.GetId() == button->GetId())
    {
        ch_box->SetValue(button->GetValue());

        event.Skip();
    }
}


bool CheckButton::GetValue() const
{
    return button->GetValue();
}


void CheckButton::SetValue(bool value)
{
    button->SetValue(value);
    ch_box->SetValue(value);
}


void CheckButton::SetToolTip(const wxString &tool_tip)
{
    wxPanel::SetToolTip(tool_tip);
    button->SetToolTip(tool_tip);
    ch_box->SetToolTip(tool_tip);
}
