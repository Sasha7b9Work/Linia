// 2025/7/24 22:47:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class WindowLibraryTests : public wxDialog
{
public:

    static const int WIDTH = 600;
    static const int HEIGHT = 450;

    WindowLibraryTests();

private:

    wxGrid *grid = nullptr;

    void SetTitlesColumn();

    void SetAutoSizeColumns();

    void Fill();

    void AppendLine(const wxString &, const wxString &, const wxString &);
};
