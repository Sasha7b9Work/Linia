// 2025/7/27 22:02:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class TableValues : public wxPanel
{
public:

    TableValues(wxWindow *);

private:

    struct Field
    {
        wxStaticText *name;
        wxTextCtrl   *value;
    };

    std::vector<Field> fields_u;
    std::vector<Field> fields_i;

    void CreateFields();
    void CreateFields_U();
    void CreateFields_I();
};
