// 2025/7/25 12:23:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/GlobalFunctions.h"
#include "MainWindow.h"
#include "Controls/DraggedWindow.h"


class Dialog : public DraggedDialog
{
public:

    Dialog(const wxString &, const wxPoint & = wxDefaultPosition, const wxSize &size = wxDefaultSize);

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
            int win_bottom = TheMainWindow->GetPosition().y + TheMainWindow->GetSize().y;     // Координата нижней кромки окна
            int win_left = TheMainWindow->GetPosition().x;

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

        DraggedDialog::ShowModal();

        TheMainWindow->HideSystemPanel();
    }

protected:

    int CreateRadioButton(wxWindow *, int x, int y, wxRadioButton **, const wxString &);

    int CreateLabelGroup(wxWindow *, int x, int y, const wxString &);

private:

    void OnKeyDown(wxKeyEvent &event)
    {
        if (event.GetKeyCode() == WXK_ESCAPE)
        {
            DraggedDialog::CloseModal();
        }
        else
        {
            event.Skip();
        }
    }
};
