// 2025/8/9 11:41:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/BmpButtonsCombo.h"


class ButtonPopup : public wxPopupTransientWindow
{
public:
    ButtonPopup(wxWindow *parent, const wxArrayString &files, int buttons_in_row) : wxPopupTransientWindow(parent, wxBORDER_SUNKEN)
    {
        // Основной контейнер с отступами по краям
        wxBoxSizer *outerSizer = new wxBoxSizer(wxVERTICAL);
        wxPanel *mainPanel = new wxPanel(this, wxID_ANY);
        mainPanel->SetBackgroundColour(*wxWHITE);

        int num_rows = (int)(files.size() / buttons_in_row);

        if (num_rows * buttons_in_row < (int)files.size())
        {
            num_rows++;
        }

        int num_cols = (int)(files.size() / num_rows);

        if (num_rows * num_cols < (int)files.size())
        {
            num_cols++;
        }

        wxGridSizer *gridSizer = new wxGridSizer(num_rows, num_cols, 5, 5); // 5px промежутки

        for (uint i = 1; i <= files.size(); ++i)
        {
            wxButton *btn = new wxButton(mainPanel, wxID_ANY, wxString::Format("Button %d", i));
            btn->Bind(wxEVT_BUTTON, &ButtonPopup::OnButtonClick, this);
            gridSizer->Add(btn, 0, wxEXPAND | wxALL, 2); // 2px отступы у кнопок
        }

        // Добавляем рамку вокруг сетки кнопок
        wxStaticBoxSizer *boxSizer = new wxStaticBoxSizer(wxVERTICAL, mainPanel, "Категория");
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
        wxMessageBox("Выбрано: " + ((wxButton *)event.GetEventObject())->GetLabel());
        Dismiss();
    }
};


BmpButtonsCombo::BmpButtonsCombo(wxWindow *parent, const wxPoint &pos, const wxSize &, const wxArrayString &_files, int num_file, int _buttons_in_row) :
    ButtonBitmap(parent, pos, wxDefaultSize, _files[(size_t)num_file])
{
    Bind(wxEVT_BUTTON, &BmpButtonsCombo::OnButtonClicked, this);

    files = _files;
    buttons_in_row = _buttons_in_row;
}


void BmpButtonsCombo::OnButtonClicked(wxCommandEvent &)
{
    ButtonPopup *popup = new ButtonPopup(this, files, buttons_in_row);

    wxPoint pos = ClientToScreen(wxPoint(GetSize().x / 2, GetSize().y / 2));
    pos.x -= popup->GetSize().x / 2;
    pos.y -= popup->GetSize().y / 2;

    popup->Position(pos, wxSize(0, 0));
    popup->Popup();
}
