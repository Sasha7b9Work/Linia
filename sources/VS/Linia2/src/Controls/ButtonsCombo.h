// 2025/8/9 11:41:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Buttons.h"

// Кнопка с рисунком, по нажатию на которую открывается окно с другими выборами


class ButtonsCombo : public wxButton
{
public:

    ButtonsCombo(wxWindow *parent, const wxString &title, const wxPoint &pos, const wxSize &,
        const wxArrayString &names,             // Эти изображения будут на кнопках
        int num_file,                           // Эта изображение будет на главной кнопке
        int buttons_in_row);                    // В каждом ряду будет расположено столько кнопок

    void SetCurrentChoice(int);

    int GetCurrentChoice() const;

private:

    int current_choice = -1;

    wxString title;
    wxArrayString names;
    int buttons_in_row;

    void OnButtonClicked(wxCommandEvent &);
};
