// 2025/7/24 22:48:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/WindowLibraryTests.h"


WindowLibraryTests::WindowLibraryTests() :
    wxDialog(nullptr, wxID_ANY, _L("Библиотека пользовательских тестов"), wxDefaultPosition, {WIDTH, HEIGHT})
{

    int dw = 200;

    {
        int width = WIDTH - dw;

        wxSize size{ width, HEIGHT };

        wxPanel *leftPanel = new wxPanel(this, wxID_ANY, { 0, 0 }, size, wxTAB_TRAVERSAL | wxSUNKEN_BORDER);

        leftPanel->SetSizeHints(width, HEIGHT, width, HEIGHT);

        grid = new wxGrid(leftPanel, wxID_ANY);
        grid->SetRowLabelSize(0);
        grid->CreateGrid(0, 3);
        SetTitlesColumn();
        grid->EnableEditing(true);
        grid->SetScrollRate(10, 10);

        grid->AutoSizeColumns();

        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(grid, 1, wxEXPAND | wxALL, 5);
        leftPanel->SetSizer(sizer);

        leftPanel->Layout();
    }

    {
        wxSize size{ dw, HEIGHT };

        wxPanel *rightPanel = new wxPanel(this, wxID_ANY, { WIDTH - dw, 0 }, size, wxTAB_TRAVERSAL | wxSUNKEN_BORDER);

        new wxButton(rightPanel, wxID_ANY, "Кнопка 1", { 10, 10 });
        new wxButton(rightPanel, wxID_ANY, "Кнопка 2", { 10, 50 });
        new wxButton(rightPanel, wxID_ANY, "Кнопка 3", { 10, 90 });
    }

    // Устанавливаем минимальный размер окна
    SetMinSize(wxSize(WIDTH, HEIGHT));

    SetSizeHints(WIDTH, HEIGHT, WIDTH, HEIGHT);

    Layout();

    SetAutoSizeColumns();

    Fill();
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
