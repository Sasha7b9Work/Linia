// 2025/08/08 21:54:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/RadioButtonICO.h"
#include "Application.h"
#include "Utils/GlobalFunctions.h"


RadioButtonICO::RadioButtonICO(wxWindow *parent, const wxPoint &position, const wxSize &size, const wxString & /*name_ico*/) :
    wxPanel(parent, wxID_ANY, position, size)
{
    button = new wxRadioButton(this, wxID_ANY, "", { 0, 0 }, size);
//    bmp = new PainterBMP(button, { 15, 8 }, { 32, 32 }, name_ico);

    button->Bind(wxEVT_RADIOBUTTON, &RadioButtonICO::OnEventRadioButton, this);
//    bmp->Bind(wxEVT_LEFT_DOWN, &RadioButtonICO::OnEventMouseDown, this);
}


void RadioButtonICO::SetValue(bool value)
{
    button->SetValue(value);
    button->Update();
}


void RadioButtonICO::OnEventRadioButton(wxCommandEvent &event)
{
    if (event.GetId() == button->GetId())
    {
        SendEvent();
    }
}


void RadioButtonICO::OnEventMouseDown(wxMouseEvent &)
{
    SendEvent();
}


void RadioButtonICO::SendEvent()
{
    GF::SendCommandEvent(this, wxEVT_RADIOBUTTON, 1);
}
