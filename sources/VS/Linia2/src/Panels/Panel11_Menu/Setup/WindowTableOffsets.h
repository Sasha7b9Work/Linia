// 2025/7/27 21:36:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Dialog.h"
#include "Device/SettingsDevice.h"

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
        wxTextCtrl   *value;
        int           range;
    };

    wxVector<Field> fields_I;
    wxVector<Field> fields_U;

    void CreateFields();
    void CreateFields_U(wxPanel *);
    void CreateFields_I(wxPanel *);

    void OnEventButton(wxCommandEvent &);
    void OnEventRadioButton(wxCommandEvent &);
    void OnEventCombobox(wxCommandEvent &);

    // Включить поля смещений
    void FillOffsets(DSet::Type::E, bool show_I);

    // Включить поля коэффициентов
    void FillK(DSet::Type::E, bool show_I);

    // Заполнить поля в соответствии с текущими установками
    void FillFields();

    bool IsChecked(int id_radiobutton) const;

    void ShowFieldsI(bool);
    void ShowFieldsU(bool);

    DSet::Type::E GetTypeMeasure() const;

    wxString GetStringValue(double) const;
};
