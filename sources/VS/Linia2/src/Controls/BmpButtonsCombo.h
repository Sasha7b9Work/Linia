// 2025/8/9 11:41:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Buttons.h"

// Кнопка с рисунком, по нажатию на которую открывается окно с другими выборами


class BmpButtonsCombo : public ButtonBitmap
{
public:

    BmpButtonsCombo(wxWindow *parent, const wxPoint &pos, const wxSize &,
        const wxArrayString &files,             // Эти изображения будут на кнопках
        int num_file,                           // Эта изображение будет на главной кнопке
        int buttons_in_row);                    // В каждом ряду будет расположено столько кнопок

private:

    wxVector<wxBitmap *> bitmaps;

    void OnButtonClicked(wxCommandEvent &);
};
