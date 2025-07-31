// 2025/7/27 21:36:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Dialog.h"

// Таблица смещений и коэффициентов


class WindowTableOffsets : public Dialog
{
public:

    static const int WIDTH = 660;
    static const int HEIGHT = 600;

    WindowTableOffsets();

private:

    struct Field
    {
        wxStaticText *name;
        wxTextCtrl *value;
    };

    wxVector<Field> fields_u;
    wxVector<Field> fields_i;

    void CreateFields();
    void CreateFields_U(wxPanel *);
    void CreateFields_I(wxPanel *);

    void OnEventButton(wxCommandEvent &);
    void OnEventRadioButton(wxCommandEvent &);
};
