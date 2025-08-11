// 2025/8/9 11:41:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/ButtonsCombo.h"


class ButtonPopup : public wxPopupTransientWindow
{
public:
    ButtonPopup(wxWindow *parent, const wxString &title, const wxArrayString &names, const wxArrayString &tooltips, int buttons_in_row) :
        wxPopupTransientWindow(parent, wxBORDER_SUNKEN)
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
            btn->SetLabel(wxString::Format("%d", i));
            btn->SetToolTip(tooltips[i]);
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

    void OnButtonClick(wxCommandEvent &event)
    {
        wxString label = ((ButtonBitmap *)event.GetEventObject())->GetLabel();

        BmpButtonsCombo *combo = (BmpButtonsCombo *)GetParent();

        int choice = -1;

        label.ToInt(&choice);

        combo->SetCurrentChoice(choice);

        Dismiss();
    }
};


ButtonsCombo::ButtonsCombo(wxWindow *parent, const wxString &_title, const wxPoint &pos, const wxSize &, const wxArrayString &_names, const wxArrayString &_tooltips, int num_name, int _buttons_in_row) :
    wxButton(parent, wxID_ANY, _names[(size_t)num_name], pos),
    current_choice(num_name)
{
    Bind(wxEVT_BUTTON, &ButtonsCombo::OnButtonClicked, this);

    SetToolTip(_tooltips[(size_t)num_name]);

    title = _title;
    names = _names;
    tooltips = _tooltips;
    buttons_in_row = _buttons_in_row;
}


void ButtonsCombo::OnButtonClicked(wxCommandEvent &)
{
    ButtonPopup *popup = new ButtonPopup(this, title, names, tooltips, buttons_in_row);

    wxPoint pos = ClientToScreen(wxPoint(GetSize().x / 2, GetSize().y / 2));
    pos.x -= popup->GetSize().x / 2;
    pos.y -= popup->GetSize().y / 2;

    popup->Position(pos, wxSize(0, 0));
    popup->Popup();
}


void ButtonsCombo::SetCurrentChoice(int choice)
{
    current_choice = choice;

    SetLabel(names[(uint)current_choice]);
}


int ButtonsCombo::GetCurrentChoice() const
{
    return current_choice;
}
