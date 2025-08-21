// 2025/8/9 11:41:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/ButtonsCombo.h"
#include "MainWindow.h"
#include "Utils/GlobalFunctions.h"
#include "Controls/StaticBox.h"


DrawingButton::DrawingButton(wxWindow *parent, int id, const wxString &label, const wxPoint &position, const wxSize &size, const wxString &_name_file) :
    wxButton(parent, id, label, position, size),
    file_name(_name_file)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT); // Для избежания мерцания

    Bind(wxEVT_PAINT, &DrawingButton::OnPaint, this);

    SetBackgroundColour(GetBackgroundColour().ChangeLightness(LIGHTNESS));
}


void DrawingButton::OnPaint(wxPaintEvent &)
{
    if (file_name[0])
    {

    }
}


class ButtonPopup : public wxPopupTransientWindow
{
public:
    ButtonPopup(wxWindow *parent) :
        wxPopupTransientWindow(parent)
    {
        wxArrayString &labels = GetCombo()->labels;

        // Основной контейнер с отступами по краям
        wxPanel *mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
//        mainPanel->SetBackgroundColour(mainPanel->GetBackgroundColour().ChangeLightness(LIGHTNESS * 2));

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

        // Добавляем рамку вокруг сетки кнопок
        StaticBoxSizer *boxSizer = new StaticBoxSizer(wxVERTICAL, mainPanel, GetCombo()->title);
        boxSizer->Add(gridSizer, 1, wxEXPAND | wxALL, 0); // 10px отступ внутри рамки

        for (uint i = 0; i < labels.size(); ++i)
        {
            if (labels[i][0])                                                           // Признак того, что надо вставлять кнопку, а не заглушку
            {
                wxButton *btn = new wxButton(mainPanel, wxID_ANY, labels[i]);
                btn->SetBackgroundColour(btn->GetBackgroundColour().ChangeLightness(LIGHTNESS * 3 / 2));
                btn->SetMaxSize(wxSize(-1, 20));
                if (labels[i] != GetCombo()->tooltips[i])
                {
                    btn->SetToolTip(GetCombo()->tooltips[i]);
                }
                btn->Bind(wxEVT_BUTTON, &ButtonPopup::OnButtonClick, this);
                gridSizer->Add(btn, 0, wxEXPAND | wxALL, 2);
            }
            else
            {
                wxPanel *panel = new wxPanel(mainPanel);
                gridSizer->Add(panel, 0, wxEXPAND | wxALL, 2);
            }
        }

        // Основная панель
        mainPanel->SetSizer(boxSizer);

        wxBoxSizer *outerSizer = new wxBoxSizer(wxVERTICAL);
        // Внешние отступы 15px
        outerSizer->Add(mainPanel, 1, wxEXPAND | wxALL, 3);
        SetSizer(outerSizer);

        Layout();

        Fit(); // Автоподбор размера

        GetParent()->Bind(wxEVT_KEY_DOWN, &ButtonPopup::OnKeyDown, this);

        Refresh();
        Update();

        SetBackgroundColour(GetBackgroundColour().ChangeLightness(50));

        // Отключаем изменение фона для всех детей
        for (auto child : GetChildren())
        {
            child->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
            child->SetBackgroundStyle(wxBG_STYLE_ERASE);
            child->Refresh(); // Обновляем внешний вид
        }

        SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY | wxWS_EX_PROCESS_UI_UPDATES);
    }

private:

    ButtonsCombo *GetCombo()
    {
        return (ButtonsCombo *)GetParent();
    }

    void OnButtonClick(wxCommandEvent &event)
    {
        wxString label = ((ButtonBitmap *)event.GetEventObject())->GetLabel();

        for (size_t i = 0; i < GetCombo()->labels.size(); i++)
        {
            if (label == GetCombo()->labels[i])
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
    const wxArrayString &_labels, const wxArrayString &_tooltips, int _buttons_in_row, Type::E type) :
    DrawingButton(parent, wxID_ANY, _labels[0], pos, { width, TEXTCNTRL_HEIGHT + 3 }, (type == Type::Bitmap) ? _title : wxString("")),
    current_choice(0)
{
    Bind(wxEVT_BUTTON, &ButtonsCombo::OnButtonClicked, this);

    title = (type == Type::Text) ? _title : wxString("");

    SetChoices(_labels, _tooltips);

    buttons_in_row = _buttons_in_row;
}


void ButtonsCombo::OnButtonClicked(wxCommandEvent &)
{
    if (labels.size() > 1)
    {
        ButtonPopup *popup = new ButtonPopup(this);

        wxPoint pos = ClientToScreen(wxPoint(GetSize().x / 2, GetSize().y / 2));

        pos.x -= popup->GetSize().x / 2;
        pos.y -= popup->GetSize().y / 2;

        popup->Position(pos, wxSize(0, 0));
        popup->Popup();
        popup->Refresh();
        popup->Update();
        popup->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    }
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
        GF::SendCommandEvent(this, wxEVT_COMBOBOX, GetCurrentSelection());
    }
}


void ButtonsCombo::SetLastSelection()
{
    SetCurrentSelection((int)labels.GetCount() - NumEmptyes() - 1);
}


int ButtonsCombo::GetCurrentSelection() const
{
    return current_choice - NumEmptyes();
}


wxString ButtonsCombo::GetCurrentString() const
{
    return labels[(uint)(current_choice + NumEmptyes())];
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


void ButtonsCombo::SetChoice(const wxString &choice)
{
    for (uint i = 0; i < labels.size(); i++)
    {
        if (labels[i] == choice)
        {
            SetCurrentSelection((int)i);
            break;
        }
    }
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
    if (start.IsEmpty())
    {
        SetLabel(end);
    }
    else
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
}


void ButtonsCombo::SetExtendedLabel(const wxString &start, int num_spaces, const wxString &end)
{
    wxString label{ start };
    label.Append(' ', (size_t)num_spaces);
    label.Append(end);
    SetLabel(label);
}


ButtonsComboRange::ButtonsComboRange(wxWindow *parent, const wxString &title, const wxPoint &pos, int width, const wxArrayString &labels, const wxArrayString &tooltips) :
    ButtonsCombo(parent, title, pos, width, labels, tooltips, 3)
{
    ButtonsCombo::insert_empty = true;

    SetChoices(labels, tooltips);
}
