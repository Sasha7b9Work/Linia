// 2025/7/24 22:48:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/WindowLibraryTests.h"


WindowLibraryTests::WindowLibraryTests() :
    wxDialog(nullptr, wxID_ANY, _L("Библиотека пользовательских тестов"), wxDefaultPosition, {WIDTH, HEIGHT})
{
    wxPanel *panel = new wxPanel(this, wxID_ANY, { 0, 0 }, { WIDTH, HEIGHT });

    wxGrid *grid = new wxGrid(panel, wxID_ANY);

    grid->SetRowLabelSize(0);

    grid->CreateGrid(0, 5);

    SetTitlesColumn();

    grid->SetScrollRate(10, 10);

    grid->AutoSizeColumns();

    // Размещаем grid в sizer для правильного масштабирования
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(grid, 1, wxEXPAND | wxALL, 5);
    panel->SetSizer(sizer);

    Layout();
}


void WindowLibraryTests::SetTitlesColumn()
{

}
