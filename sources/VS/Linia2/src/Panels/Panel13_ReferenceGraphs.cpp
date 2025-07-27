// 2025/7/26 23:56:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel13_ReferenceGraphs.h"
#include "MainWindow.h"


PanelReferenceGraph *PanelReferenceGraph::self = nullptr;


MyGrid::MyGrid(wxWindow *parent) : wxGrid(parent, wxID_ANY)
{
    // На такие ухищрения приходится идти потому, что wxGrid не принимает напрямую wxEVT_RIGTH_DOWN и не пропускает их дальше

    GetGridWindow()->Bind(wxEVT_RIGHT_DOWN, [](wxMouseEvent &event)
        {
            wxMouseEvent panelEvent(event);
            panelEvent.SetEventObject(PanelReferenceGraph::self);
            PanelReferenceGraph::self->ProcessWindowEvent(panelEvent);

            event.Skip();
        });
}


PanelReferenceGraph::PanelReferenceGraph(wxWindow *parent) :
    Panel(parent, 0, 0, MainWindow::WIDTH, MainWindow::HEIGHT)
{
    self = this;

    int height_title = BUTTON_HEIGHT;

    {
        wxPanel *panel = new wxPanel(this, wxID_ANY, { 0, 0 }, { MainWindow::WIDTH, height_title });

        wxSize size_button{ 75, BUTTON_HEIGHT };
        new wxButton(panel, ID_BTN_RETURN_TO_MAIN_PAGE, _L("Закрыть"), { MainWindow::WIDTH - size_button.x - 5, 0 }, size_button);

        new wxStaticText(panel, wxID_ANY, _L("Архив эталонных графиков"), { 0, 0 }, { MainWindow::WIDTH - size_button.x - 5, height_title }, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);
    }

    {
        wxPanel *panel = new wxPanel(this, wxID_ANY, { 0, height_title }, { MainWindow::WIDTH, MainWindow::HEIGHT - height_title });

        grid = new MyGrid(panel);

        grid->EnableEditing(false);
        grid->EnableGridLines(false);
        grid->EnableDragCell(false);
        grid->EnableCellEditControl(false);

        grid->SetRowLabelSize(0);

        grid->CreateGrid(0, 5);

        SetTitlesColumn();

        grid->SetScrollRate(10, 10);

        grid->AutoSizeColumns();

        // Размещаем grid в sizer для правильного масштабирования
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(grid, 1, wxEXPAND | wxALL, 5);
        panel->SetSizer(sizer);

        panel->Layout();

        SetAutoSizeColumns();

        CreateTestLines();

        UpdateLines();

        grid->SetSelectionMode(wxGrid::wxGridSelectNone);

        grid->SetCellHighlightPenWidth(0);
        grid->SetCellHighlightROPenWidth(0);

        grid->SetSelectionMode(wxGrid::wxGridSelectRows);
    }

    Bind(wxEVT_BUTTON, &PanelReferenceGraph::OnEventButton, this);
    Bind(wxEVT_RIGHT_DOWN, &PanelReferenceGraph::OnEventRightClick, this);
    Bind(wxEVT_GRID_CELL_RIGHT_CLICK, &PanelReferenceGraph::OnEventGridClick, this);
}


void PanelReferenceGraph::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_BTN_RETURN_TO_MAIN_PAGE)
    {
        MainWindow::self->SetMode(ModeMainWindow::Standard);
    }
}


void PanelReferenceGraph::OnEventRightClick(wxMouseEvent &event)
{
    wxMenu menu;

    wxMenuItem *item = menu.Append(ID_MENU_CLEAR, _L("Очистить архив"));

    if (grid->GetNumberRows() == 0)
    {
        item->Enable(false);
    }

    menu.AppendSeparator();
    item = menu.Append(ID_MENU_DELETE_FROM, _L("Удалить из архива"));

    wxArrayInt selected = grid->GetSelectedRows();

    if (selected.IsEmpty())
    {
        item->Enable(false);
    }

    item = menu.Append(ID_MENU_LOAD_FROM, _L("Загрузить из архива"));

    if (selected.IsEmpty())
    {
        item->Enable(false);
    }

    menu.AppendSeparator();
    menu.Append(ID_BTN_RETURN_TO_MAIN_PAGE, _("Закрыть архив"));

    Bind(wxEVT_MENU, &PanelReferenceGraph::OnEventMenu, this);

    PopupMenu(&menu);

    event.Skip();
}


void PanelReferenceGraph::OnEventMenu(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_BTN_RETURN_TO_MAIN_PAGE)
    {
        MainWindow::self->SetMode(ModeMainWindow::Standard);
    }
}


void PanelReferenceGraph::OnEventGridClick(wxGridEvent &event)
{
    int id = event.GetId();

    id = id;
}


void PanelReferenceGraph::SetTitlesColumn()
{
    titles_columns.Add("№");
    titles_columns.Add(_L("График"));
    titles_columns.Add(_L("Дата"));
    titles_columns.Add(_L("Время"));
    titles_columns.Add(_L("Комментарий"));

    for (uint i = 0; i < 5; i++)
    {
        grid->SetColLabelValue((int)i, titles_columns[i]);
    }
}


void PanelReferenceGraph::SetAutoSizeColumns()
{
    int gridWidth = grid->GetClientSize().GetWidth();
    int colsCount = grid->GetNumberCols();
    int colWidth = (gridWidth - grid->GetRowLabelSize()) / colsCount;

    for (int col = 0; col < colsCount; ++col)
    {
        grid->SetColSize(col, colWidth);
    }
}


void PanelReferenceGraph::CreateTestLines()
{
    wxDateTime date;
//    date.Set(9, wxDateTime::Oct, 2015, 16, 3, 44);

    lines.push_back({ 1, "Forward", date.Set(9, wxDateTime::Oct, 2025, 16, 3, 44), "ВАХ в прямом включении перехода" });
    lines.push_back({ 2, "IdVd", date.Set(12, wxDateTime::Oct, 2025, 10, 13, 2), "Выходная характеристика" });
}


void PanelReferenceGraph::UpdateLines()
{
    if (grid->GetNumberRows() < (int)lines.size())
    {
        grid->AppendRows((int)lines.size() - grid->GetNumberRows());
    }

    for (int i = 0; i < (int)lines.size(); i++)
    {
        const Line &l = lines[(uint)i];

        grid->SetCellValue(i, 0, wxString::Format("%d", l.number));
        grid->SetCellValue(i, 1, l.graphic);
        grid->SetCellValue(i, 2, l.time.Format("%d.%m.%Y"));
        grid->SetCellValue(i, 3, l.time.Format("%H:%M:%S"));
        grid->SetCellValue(i, 4, l.comment);
    }
}

