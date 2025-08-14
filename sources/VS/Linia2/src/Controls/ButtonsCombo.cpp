// 2025/8/9 11:41:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/ButtonsCombo.h"
#include "MainWindow.h"


class ButtonPopup : public wxPopupTransientWindow
{
public:
    ButtonPopup(wxWindow *parent, const wxString &title, const wxArrayString &_names, int buttons_in_row) :
        wxPopupTransientWindow(parent, wxBORDER_SIMPLE),
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

        wxGridSizer *gridSizer = new wxGridSizer(num_rows, num_cols, 2, 2); // 5px промежутки

        for (uint i = 0; i < names.size(); ++i)
        {
            if (names[i][0])
            {
                wxButton *btn = new wxButton(mainPanel, wxID_ANY, names[i]);
                btn->Bind(wxEVT_BUTTON, &ButtonPopup::OnButtonClick, this);
                gridSizer->Add(btn, 0, wxEXPAND | wxALL, 0); // 2px отступы у кнопок
            }
            else
            {
                wxPanel *panel = new wxPanel(mainPanel);
                gridSizer->Add(panel, 0, wxEXPAND | wxALL, 0);
            }
        }

        // Добавляем рамку вокруг сетки кнопок
        wxStaticBoxSizer *boxSizer = new wxStaticBoxSizer(wxVERTICAL, mainPanel, title);
        boxSizer->Add(gridSizer, 1, wxEXPAND | wxALL, 5); // 10px отступ внутри рамки

        // Основная панель
        mainPanel->SetSizer(boxSizer);

        // Внешние отступы 15px
        outerSizer->Add(mainPanel, 1, wxEXPAND | wxALL, 10);
        SetSizer(outerSizer);

        Fit(); // Автоподбор размера

        GetParent()->Bind(wxEVT_KEY_DOWN, &ButtonPopup::OnKeyDown, this);
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

                combo->SetCurrentSelection((int)i - combo->NumEmptyes());

                Dismiss();

                break;
            }
        }
    }

    void OnKeyDown(wxKeyEvent &event)
    {
        if (event.GetKeyCode() != WXK_SPACE)
        {
            Dismiss();
        }

        event.Skip();
    }
};


ButtonsCombo::ButtonsCombo(wxWindow *parent, const wxString &_title, const wxPoint &pos, int width, const wxArrayString &_names, int _buttons_in_row, bool _insert_empty) :
    wxButton(parent, wxID_ANY, _names[0], pos, { width, TEXTCNTRL_HEIGHT + 3 }),
    current_choice(0),
    insert_empty(_insert_empty)
{
    Bind(wxEVT_BUTTON, &ButtonsCombo::OnButtonClicked, this);

    title = _title;

    SetChoices(_names);

    buttons_in_row = _buttons_in_row;
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


void ButtonsCombo::SetCurrentSelection(int choice)
{
    bool need_event = (choice != current_choice);

    current_choice = choice;

    wxString label;

    if (title[0])
    {
        label += title + " : ";
    }

    SetExtendedLabel(label, names[(uint)current_choice + NumEmptyes()]);

    if(need_event)
    {
        wxCommandEvent event(wxEVT_COMBOBOX, GetId());
        event.SetEventObject(this);
        event.SetInt(GetCurrentSelection());
        wxPostEvent(GetEventHandler(), event);
    }
}


int ButtonsCombo::GetCurrentSelection() const
{
    return current_choice - NumEmptyes();
}


wxString ButtonsCombo::GetCurrentString() const
{
    return names[(size_t)GetCurrentSelection()];
}


void ButtonsCombo::SetChoices(const wxArrayString &choices)
{
    names.clear();

    if (insert_empty)
    {
        if (choices[0][0] == '2')
        {
            names.push_back("");
        }
        else if (choices[0][0] == '4' || choices[0][0] == '5')
        {
            names.push_back("");
            names.push_back("");
        }
    }

    for (auto &elem : choices)
    {
        names.push_back(elem);
    }

    SetCurrentSelection(0);
}


int ButtonsCombo::NumEmptyes() const
{
    int counter = 0;

    for (auto &elem : names)
    {
        if (elem[0] == '\0')
        {
            counter++;
        }
        else
        {
            break;
        }
    }

    return counter;
}


void ButtonsCombo::SetExtendedLabel(const wxString &start, const wxString &end)
{
    int num_spaces = 0;

    while (true)
    {
        SetExtendedLabel(start, num_spaces, end);

        wxSize size = GetTextExtent(GetLabel());

        if (size.x >= GetClientSize().x - 12)
        {
            if (num_spaces > 0)
            {
                num_spaces--;
            }

            break;
        }

        num_spaces++;
    }

    SetExtendedLabel(start, num_spaces, end);
}


void ButtonsCombo::SetExtendedLabel(const wxString &start, int num_spaces, const wxString &end)
{
    wxString label{ start };
    label.Append(' ', (size_t)num_spaces);
    label.Append(end);
    SetLabel(label);
}
