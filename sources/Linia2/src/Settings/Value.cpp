// 2026/03/17 17:32:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Settings/Value.h"
#include "Settings/Settings.h"


ValueBool::ValueBool(const wxString &_key, bool _def) : Value<bool>(_key, _def)
{
}


ValueCheckBox::ValueCheckBox(wxWindow *parent, const wxString &title, const wxString &_key, bool _def) :
    wxCheckBox(parent, wxID_ANY, title)
{
    value = new ValueBool(_key, _def);

    value->Load();

    SetValue(value->Get());
}


void ValueCheckBox::SetNewValue(bool val)
{
    SetValue(val);

    wxCommandEvent event(wxEVT_CHECKBOX, GetId());
    event.SetEventObject(this);
    event.SetInt(GetValue());
    wxPostEvent(GetEventHandler(), event);
}


void ValueCheckBox::SetValue(bool val)
{
    wxCheckBox::SetValue(val);
}


ValueInt::ValueInt(const wxString &_key, int _def) : Value<int>(_key, _def)
{
}


ValueUInt::ValueUInt(const wxString &_key, uint _def) : Value<uint>(_key, _def)
{
}


ValuePoint::ValuePoint(const wxString &_key, const wxPoint &_def) : Value<wxPoint>(_key, _def)
{
}
