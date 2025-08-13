// 2025/8/9 11:41:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/ButtonsCombo.h"


class ButtonPopup : public wxPopupTransientWindow
{
public:
    ButtonPopup(wxWindow *parent, const wxString &title, const wxArrayString &_names, int buttons_in_row) :
        wxPopupTransientWindow(parent, wxBORDER_SUNKEN),
        names(_names)
    {
        // Основной контейнер с отступами по краям
        wxBoxSizer *outerSizer = new wxBoxSizer(wxVERTICAL);
        wxPanel *mainPanel = new wxPanel(this, wxID_ANY);
        mainPanel->SetBackgroundColour(*wxWHITE);

        int num_rows = (int)(names.size() / buttons_in_row);

        if (num_rows * buttons_in_row < (int)names.size())
        {
            num_rows++;
        }

        int num_cols = (int)(names.size() / num_rows);

        if (num_rows * num_cols < (int)names.size())
        {
            num_cols++;
        }

        wxGridSizer *gridSizer = new wxGridSizer(num_rows, num_cols, 5, 5); // 5px промежутки

        for (uint i = 0; i < names.size(); ++i)
        {
            wxButton *btn = new wxButton(mainPanel, wxID_ANY, names[i]);
            btn->Bind(wxEVT_BUTTON, &ButtonPopup::OnButtonClick, this);
            gridSizer->Add(btn, 0, wxEXPAND | wxALL, 2); // 2px отступы у кнопок
        }

        // Добавляем рамку вокруг сетки кнопок
        wxStaticBoxSizer *boxSizer = new wxStaticBoxSizer(wxVERTICAL, mainPanel, title);
        boxSizer->Add(gridSizer, 1, wxEXPAND | wxALL, 0); // 10px отступ внутри рамки

        // Основная панель
        mainPanel->SetSizer(boxSizer);

        // Внешние отступы 15px
        outerSizer->Add(mainPanel, 1, wxEXPAND | wxALL, 5);
        SetSizer(outerSizer);

        Fit(); // Автоподбор размера
    }

private:

    wxArrayString names;

    void OnButtonClick(wxCommandEvent &event)
    {
        wxString label = ((ButtonBitmap *)event.GetEventObject())->GetLabel();

        for (size_t i = 0; i < names.size(); i++)
        {
            if (label == names[i])
            {
                ButtonsCombo *combo = (ButtonsCombo *)GetParent();

                combo->SetCurrentChoice((int)i);

                Dismiss();

                break;
            }
        }
    }
};


ButtonsCombo::ButtonsCombo(wxWindow *parent, const wxString &_title, const wxPoint &pos, const wxSize &size, const wxArrayString &_names, int num_name, int _buttons_in_row) :
    wxButton(parent, wxID_ANY, _names[(size_t)num_name], pos, size),
    current_choice(num_name)
{
    Bind(wxEVT_BUTTON, &ButtonsCombo::OnButtonClicked, this);

    title = _title;
    names = _names;
    buttons_in_row = _buttons_in_row;

    SetCurrentChoice(num_name);
}


void ButtonsCombo::OnButtonClicked(wxCommandEvent &)
{
    ButtonPopup *popup = new ButtonPopup(this, title, names, buttons_in_row);

    wxPoint pos = ClientToScreen(wxPoint(GetSize().x / 2, GetSize().y / 2));
    pos.x -= popup->GetSize().x / 2;
    pos.y -= popup->GetSize().y / 2;

    popup->Position(pos, wxSize(0, 0));
    popup->Popup();
}


void ButtonsCombo::SetCurrentChoice(int choice)
{
    current_choice = choice;

    SetLabel(title + ": " + names[(uint)current_choice]);
}


int ButtonsCombo::GetCurrentChoice() const
{
    return current_choice;
}
