// 2025/7/27 22:02:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct TableStruct
{
    wxString name;
    double value;
};


class TableValues : public wxPanel
{
public:

    TableValues(wxWindow *);

    void SetAll(wxVector<TableStruct> &);

private:

    wxVector<TableStruct> values;
};
