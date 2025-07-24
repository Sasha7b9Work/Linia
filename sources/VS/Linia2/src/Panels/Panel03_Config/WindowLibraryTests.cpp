// 2025/7/24 22:48:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/WindowLibraryTests.h"


WindowLibraryTests::WindowLibraryTests() :
    wxDialog(nullptr, wxID_ANY, _L("Библиотека пользовательских тестов"), wxDefaultPosition, {WIDTH, HEIGHT})
{
    grid = new wxGrid(this, wxID_ANY);

    grid->EnableEditing(false);

    grid->SetRowLabelSize(0);

    grid->CreateGrid(0, 3);

    SetTitlesColumn();

    grid->SetScrollRate(10, 10);

    grid->AutoSizeColumns();

    // Размещаем grid в sizer для правильного масштабирования
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(grid, 1, wxEXPAND | wxALL, 5);
    SetSizer(sizer);

    wxTopLevelWindowBase::Layout();

    SetAutoSizeColumns();

    Fill();

    grid->SetSelectionMode(wxGrid::wxGridSelectNone);
}


void WindowLibraryTests::SetTitlesColumn()
{
    grid->SetColLabelValue(0, "#");
    grid->SetColLabelValue(1, _L("Наименование файла"));
    grid->SetColLabelValue(2, _L("Комментарий"));
}


void WindowLibraryTests::SetAutoSizeColumns()
{
    int gridWidth = grid->GetClientSize().GetWidth();
    int colsCount = grid->GetNumberCols();
    int colWidth = (gridWidth - grid->GetRowLabelSize()) / colsCount;

    for (int col = 0; col < colsCount; ++col)
    {
        grid->SetColSize(col, colWidth);
    }
}


void WindowLibraryTests::Fill()
{
    AppendLine("1", "IdVd_IRF840.tst", "Выходная характеристика");
}


void WindowLibraryTests::AppendLine(const wxString &c0, const wxString &c1, const wxString &c2)
{
    grid->AppendRows(1);

    int row = grid->GetNumberRows() - 1;

    grid->SetCellValue(row, 0, c0);
    grid->SetCellValue(row, 1, c1);
    grid->SetCellValue(row, 2, c2);
}
