// 2025/8/9 11:41:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/ButtonsCombo.h"
#include "MainWindow.h"


class ButtonPopup : public wxPopupTransientWindow
{
public:
    ButtonPopup(wxWindow *parent, const wxString &title, const wxArrayString &_labels, const wxArrayString &tooltips) :
        wxPopupTransientWindow(parent, wxBORDER_SIMPLE),
        labels(_labels)
    {
        // Основной контейнер с отступами по краям
        wxBoxSizer *outerSizer = new wxBoxSizer(wxVERTICAL);
        wxPanel *mainPanel = new wxPanel(this, wxID_ANY);
        mainPanel->SetBackgroundColour(*wxWHITE);

        int num_rows = (int)(labels.size() / GetCombo()->buttons_in_row);

        if (num_rows * GetCombo()->buttons_in_row < (int)labels.size())
        {
            num_rows++;
        }

        int num_cols = (int)(labels.size() / num_rows);

        if (num_rows * num_cols < (int)labels.size())
        {
            num_cols++;
        }

        wxGridSizer *gridSizer = new wxGridSizer(num_rows, num_cols, 2, 2);

        for (uint i = 0; i < labels.size(); ++i)
        {
            if (labels[i][0])                                                           // Признак того, что надо вставлять кнопку, а не заглушку
            {
                wxButton *btn = new wxButton(mainPanel, wxID_ANY, labels[i]);
                if (labels[i] != tooltips[i])
                {
                    btn->SetToolTip(tooltips[i]);
                }
                btn->Bind(wxEVT_BUTTON, &ButtonPopup::OnButtonClick, this);
                gridSizer->Add(btn, 0, wxEXPAND | wxALL, 0);
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

    ButtonsCombo *GetCombo()
    {
        return (ButtonsCombo *)GetParent();
    }

    wxArrayString labels;

    void OnButtonClick(wxCommandEvent &event)
    {
        wxString label = ((ButtonBitmap *)event.GetEventObject())->GetLabel();

        for (size_t i = 0; i < labels.size(); i++)
        {
            if (label == labels[i])
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


ButtonsCombo::ButtonsCombo(wxWindow *parent, const wxString &_title, const wxPoint &pos, int width,
    const wxArrayString &_labels, const wxArrayString &_tooltips, int _buttons_in_row, bool _insert_empty) :
    wxButton(parent, wxID_ANY, _labels[0], pos, { width, TEXTCNTRL_HEIGHT + 3 }),
    current_choice(0),
    insert_empty(_insert_empty)
{
    Bind(wxEVT_BUTTON, &ButtonsCombo::OnButtonClicked, this);

    title = _title;

    SetChoices(_labels, _tooltips);

    buttons_in_row = _buttons_in_row;
}


void ButtonsCombo::OnButtonClicked(wxCommandEvent &)
{
    ButtonPopup *popup = new ButtonPopup(this, title, labels, tooltips);

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

    uint index = (uint)current_choice + NumEmptyes();

    SetExtendedLabel(label, labels[index]);

    SetToolTip((tooltips[index] != labels[index]) ? (tooltips[index]) : wxString(""));

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
    return labels[(size_t)GetCurrentSelection()];
}


void ButtonsCombo::SetChoices(const wxArrayString &choices, const wxArrayString &_tooltips)
{
    labels.clear();
    tooltips.clear();

    if (insert_empty)
    {
        if (choices[0][0] == '2')
        {
            labels.push_back("");
            tooltips.push_back("");
        }
        else if (choices[0][0] == '4' || choices[0][0] == '5')
        {
            labels.push_back("");
            labels.push_back("");
            tooltips.push_back("");
            tooltips.push_back("");
        }
    }

    for (auto &elem : choices)
    {
        labels.push_back(elem);
    }

    for (auto &elem : _tooltips)
    {
        tooltips.push_back(elem);
    }

    SetCurrentSelection(0);
}


int ButtonsCombo::NumEmptyes() const
{
    int counter = 0;

    for (auto &elem : labels)
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
