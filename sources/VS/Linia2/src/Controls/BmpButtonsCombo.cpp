// 2025/8/9 11:41:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/BmpButtonsCombo.h"


class ButtonPopup : public wxPopupTransientWindow
{
public:
    ButtonPopup(wxWindow *parent, const wxArrayString &files, int buttons_in_row) : wxPopupTransientWindow(parent)
    {
        // Создаем основной sizer с горизонтальным расположением столбцов
        wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);

        // Создаем 3 вертикальных sizer'а для столбцов
        wxBoxSizer *column1 = new wxBoxSizer(wxVERTICAL);
        wxBoxSizer *column2 = new wxBoxSizer(wxVERTICAL);
        wxBoxSizer *column3 = new wxBoxSizer(wxVERTICAL);

        // Создаем 10 кнопок и распределяем по столбцам
        for (int i = 1; i <= 10; ++i)
        {
            wxButton *btn = new wxButton(this, wxID_ANY, wxString::Format("Button %d", i));
            btn->Bind(wxEVT_BUTTON, &ButtonPopup::OnButtonClick, this);

            // Распределяем кнопки по столбцам: 4-3-3
            if (i <= 4)
            {
                column1->Add(btn, 0, wxEXPAND | wxALL, 5);
            }
            else if (i <= 7)
            {
                column2->Add(btn, 0, wxEXPAND | wxALL, 5);
            }
            else
            {
                column3->Add(btn, 0, wxEXPAND | wxALL, 5);
            }
        }

        // Добавляем столбцы в основной sizer с отступами
        mainSizer->Add(column1, 0, wxEXPAND | wxALL, 5);
        mainSizer->Add(column2, 0, wxEXPAND | wxALL, 5);
        mainSizer->Add(column3, 0, wxEXPAND | wxALL, 5);

        SetSizer(mainSizer);
        Fit(); // Автоматически подгоняем размер
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
