// 2025/8/9 11:41:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/BmpButtonsCombo.h"


class ButtonPopup : public wxPopupTransientWindow
{
public:
    ButtonPopup(wxWindow *parent) : wxPopupTransientWindow(parent)
    {
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

        for (int i = 1; i <= 3; ++i)
        {
            wxButton *btn = new wxButton(this, wxID_ANY, wxString::Format("Option %d", i));
            btn->Bind(wxEVT_BUTTON, &ButtonPopup::OnButtonClick, this);
            sizer->Add(btn, 0, wxEXPAND | wxALL, 5);
        }

        SetSizer(sizer);
        Fit();
    }

private:
    void OnButtonClick(wxCommandEvent &event)
    {
        wxMessageBox("Выбрано: " + ((wxButton *)event.GetEventObject())->GetLabel());
        Dismiss();
    }
};


BmpButtonsCombo::BmpButtonsCombo(wxWindow *parent, const wxPoint &pos, const wxSize &, const wxArrayString &files, int num_file, int buttons_in_row) :
    ButtonBitmap(parent, pos, wxDefaultSize, files[(size_t)num_file])
{
    Bind(wxEVT_BUTTON, &BmpButtonsCombo::OnButtonClicked, this);
}


void BmpButtonsCombo::OnButtonClicked(wxCommandEvent &)
{
    ButtonPopup *popup = new ButtonPopup(this);
    wxPoint pos = ClientToScreen(wxPoint(-50, -50));
    popup->Position(pos, wxSize(0, 0));
    popup->Popup();
}
