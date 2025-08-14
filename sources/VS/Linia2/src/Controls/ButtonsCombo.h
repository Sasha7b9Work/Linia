// 2025/8/9 11:41:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Buttons.h"

// Кнопка с рисунком, по нажатию на которую открывается окно с другими выборами


class ButtonsCombo : public wxButton
{
    friend class ButtonPopup;

public:

    explicit ButtonsCombo(wxWindow *parent, const wxString &title, const wxPoint &pos, int width,
        const wxArrayString &names,             // Эти изображения будут на кнопках
        int buttons_in_row,                     // В каждом ряду будет расположено столько кнопок
        bool insert_empty);

    void SetCurrentSelection(int);

    void SetChoices(const wxArrayString &);

    int GetCurrentSelection() const;

    wxString GetCurrentString() const;

private:

    int current_choice = -1;
    bool insert_empty = false;          // true, если перед первым элементом нужно вставлять пустые элементы (для диапазонов, чтобы они согласованно располагались по столбцам)
    wxString title;
    wxArrayString names;
    int buttons_in_row;

    void OnButtonClicked(wxCommandEvent &);

    // Возвращает количество начальных пустых элементов
    int NumEmptyes() const;

    // Между строками будут пробелы таким образом, что строки будут по краям кнопки
    void SetExtendedLabel(const wxString &, const wxString &);
    void SetExtendedLabel(const wxString &, int num_spaces, const wxString &);
};
