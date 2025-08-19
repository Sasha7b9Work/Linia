// 2025/8/9 11:41:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Buttons.h"


// Кнопка с рисунком, по нажатию на которую открывается окно с другими выборами


class DrawingButton : public wxButton
{
public:
    DrawingButton(wxWindow *parent, int id, const wxString &, const wxPoint &, const wxSize &, const wxString &name_file = "");

private:

    wxString file_name;

    void OnPaint(wxPaintEvent &);
};


class ButtonsCombo : public DrawingButton
{
    friend class ButtonPopup;

public:

    struct Type
    {
        enum E
        {
            Text,
            Bitmap
        };
    };

    explicit ButtonsCombo(wxWindow *parent, const wxString &title, const wxPoint &pos, int width,
        const wxArrayString &labels,
        const wxArrayString &tooltips,
        int buttons_in_row,                     // В каждом ряду будет расположено столько кнопок
        Type::E type = Type::Text);

    void SetCurrentSelection(int);

    void SetLastSelection();

    void SetChoices(const wxArrayString &labels, const wxArrayString &tooltips);

    void SetChoice(const wxString &);

    int GetCurrentSelection() const;

    wxString GetCurrentString() const;

protected:

    bool insert_empty = false;          // true, если перед первым элементом нужно вставлять пустые элементы (для диапазонов, чтобы они согласованно располагались по столбцам)

private:

    int current_choice = -1;
    wxString title;
    wxArrayString labels;
    wxArrayString tooltips;
    int buttons_in_row;

    void OnButtonClicked(wxCommandEvent &);

    // Возвращает количество начальных пустых элементов
    int NumEmptyes() const;

    // Между строками будут пробелы таким образом, что строки будут по краям кнопки
    void SetExtendedLabel(const wxString &, const wxString &);
    void SetExtendedLabel(const wxString &, int num_spaces, const wxString &);
};


class ButtonsComboRange : public ButtonsCombo
{
public:

    ButtonsComboRange(wxWindow *parent, const wxString &title, const wxPoint &pos, int width,
        const wxArrayString &labels,
        const wxArrayString &tooltips);

private:
};
