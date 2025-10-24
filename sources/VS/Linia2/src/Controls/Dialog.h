// 2025/7/25 12:23:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/GlobalFunctions.h"
#include "MainWindow.h"


class Dialog : public wxDialog
{
public:

    Dialog(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize) :
        wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP)
    {
        Bind(wxEVT_CHAR_HOOK, &Dialog::OnKeyDown, this);
    }

    virtual int ShowModal() override
    {
        wxPoint pos = GF::GetCoordCenter(GetSize());

        SetPosition(pos);

        int result = wxDialog::ShowModal();

        MainWindow::self->Raise();

        return result;
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
            EndModal(wxID_CANCEL);
        }
        else
        {
            event.Skip();
        }
    }
};
