// 2025/8/9 11:41:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/BmpButtonsCombo.h"


BmpButtonsCombo::BmpButtonsCombo(wxWindow *parent, const wxString &title, const wxPoint &pos, const wxSize &, const wxArrayString &files, const wxArrayString &tooltips, int num_file, int buttons_in_row) :
    ButtonBitmap(parent, pos, wxDefaultSize, files[(size_t)num_file])
{
    Bind(wxEVT_BUTTON, &BmpButtonsCombo::OnButtonClicked, this);

    SetToolTip(tooltips[(size_t)num_file]);

    popup = new ButtonPopup(this, title, files, tooltips, buttons_in_row);

    popup->Hide();
}


void BmpButtonsCombo::OnButtonClicked(wxCommandEvent &)
{
    wxPoint pos = ClientToScreen(wxPoint(GetSize().x / 2, GetSize().y / 2));
    pos.x -= popup->GetSize().x / 2;
    pos.y -= popup->GetSize().y / 2;

    popup->ShowPopup(pos);
}
