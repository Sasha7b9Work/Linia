// 2025/8/9 11:41:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/BmpButtonsCombo.h"


class ButtonPopup : public wxPopupTransientWindow
{
public:
    ButtonPopup(wxWindow *parent, const wxArrayString &files, int buttons_in_row) : wxPopupTransientWindow(parent, wxBORDER_SUNKEN)
    {
        const int BUTTONS_COUNT = 10;
        const int COLUMNS_COUNT = 3;

        // Рассчитываем количество строк
        int rows = (BUTTONS_COUNT + COLUMNS_COUNT - 1) / COLUMNS_COUNT;

        wxGridSizer *gridSizer = new wxGridSizer(rows, COLUMNS_COUNT, 5, 5);

        // Добавляем кнопки по строкам
        for (int i = 1; i <= BUTTONS_COUNT; ++i)
        {
            wxButton *btn = new wxButton(this, wxID_ANY, wxString::Format("Button %d", i));
            btn->Bind(wxEVT_BUTTON, &ButtonPopup::OnButtonClick, this);
            gridSizer->Add(btn, 0, wxEXPAND);
        }

        // Добавляем пустые места, если нужно
        for (int i = BUTTONS_COUNT + 1; i <= rows * COLUMNS_COUNT; ++i)
        {
            gridSizer->AddStretchSpacer();
        }

        SetSizer(gridSizer);
        Fit();
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
    wxPoint pos = ClientToScreen(wxPoint(-50, -50));
    popup->Position(pos, wxSize(0, 0));
    popup->Popup();
}
