// 2025/7/24 22:48:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/WindowLibraryTests.h"


WindowLibraryTests::WindowLibraryTests() :
    wxDialog(nullptr, wxID_ANY, _L("Библиотека пользовательских тестов"), wxDefaultPosition, {WIDTH, HEIGHT})
{
    // Создаем основной sizer для диалога
    wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);

    // Левая панель с wxGrid
    wxPanel *leftPanel = new wxPanel(this, wxID_ANY);

    wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);

    {
        grid = new wxGrid(leftPanel, wxID_ANY);
        grid->SetRowLabelSize(0);
        grid->CreateGrid(0, 3);
        SetTitlesColumn();
        grid->EnableEditing(true);
        grid->SetScrollRate(10, 10);

        grid->AutoSizeColumns();

        leftSizer->Add(grid, 1, wxEXPAND | wxALL, 5);
        leftPanel->SetSizer(leftSizer);
    }

    // Правая панель с кнопками
    wxPanel *rightPanel = new wxPanel(this, wxID_ANY);
    wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);

    {
        wxButton *button1 = new wxButton(rightPanel, wxID_ANY, "Кнопка 1");
        wxButton *button2 = new wxButton(rightPanel, wxID_ANY, "Кнопка 2");
        wxButton *button3 = new wxButton(rightPanel, wxID_ANY, "Кнопка 3");

        rightSizer->Add(button1, 0, wxEXPAND | wxALL, 5);
        rightSizer->Add(button2, 0, wxEXPAND | wxALL, 5);
        rightSizer->Add(button3, 0, wxEXPAND | wxALL, 5);
        rightSizer->AddStretchSpacer();

        rightPanel->SetSizer(rightSizer);
    }

    // Добавляем панели в основной sizer
    mainSizer->Add(leftPanel, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(rightPanel, 0, wxEXPAND | wxALL, 5);

    SetSizer(mainSizer);

    // Устанавливаем минимальный размер окна
    SetMinSize(wxSize(WIDTH, HEIGHT));

    SetSizeHints(WIDTH, HEIGHT, WIDTH, HEIGHT);

    Layout();

    SetAutoSizeColumns();
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
