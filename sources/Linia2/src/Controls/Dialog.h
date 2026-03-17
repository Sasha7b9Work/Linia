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

    void ShowOnWindow(wxWindow *window)
    {
        wxSize size = window->GetSize();

        wxPoint pos = window->ClientToScreen(wxPoint(size.x / 2, size.y / 2));

        pos -= GetSize() / 2;

        if (GF::IsBoardPCM())
        {

        }
        else
        {
            int win_bottom = MainWindow::self->GetPosition().y + MainWindow::self->GetSize().y;     // Координата нижней кромки окна
            int win_left = MainWindow::self->GetPosition().x;

            int bottom = pos.y + GetSize().y;
            int left = pos.x;

            if (bottom >= win_bottom)
            {
                pos.y -= (bottom - win_bottom);
            }
            if (left <= win_left)
            {
                pos.x += (win_left - left);
            }
        }

        SetPosition(pos);

        wxDialog::ShowModal();

        MainWindow::self->HideSystemPanel();
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
