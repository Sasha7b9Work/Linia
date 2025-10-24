// 2025/7/25 12:23:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/GlobalFunctions.h"
#include "Controls/ButtonsCombo.h"


class Dialog : public ButtonsCombo
{
public:

    Dialog(wxWindow *parent, const wxString &title, const wxPoint &pos = wxDefaultPosition, const wxSize & = wxDefaultSize) :
        ButtonsCombo(parent, title, pos, 100, wxArrayString(), wxArrayString(), 1, title)
    {
        Bind(wxEVT_CHAR_HOOK, &Dialog::OnKeyDown, this);
    }

    int ShowModal()
    {
//        wxPoint pos = GF::GetCoordCenter(GetSize());
//
//        SetPosition(pos);
//
//        return wxDialog::ShowModal();
        return 0;
    }

protected:

    int CreateRadioButton(wxWindow *, int x, int y, wxRadioButton **, const wxString &);

    int CreateLabelGroup(wxWindow *, int x, int y, const wxString &);

private:

    void OnKeyDown(wxKeyEvent &event)
    {
        if (event.GetKeyCode() == WXK_ESCAPE)
        {
            // Close();
//            EndModal(wxID_CANCEL);
        }
        else
        {
            event.Skip();
        }
    }
};
