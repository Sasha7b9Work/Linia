// 2025/6/1 18:08:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "Panels/Panel11_Menu/Panel11_Menu.h"


class PanelTable : public Panel
{
public:

    static const int WIDTH = MainWindow::WIDTH2;
    static const int HEIGHT = MainWindow::HEIGHT - MainWindow::HEIGTH1 - PanelGraph::HEIGHT - PanelButtons::HEIGHT;

    PanelTable(wxWindow *parent);

    static PanelTable *self;

private:

    wxGrid *grid;

    wxArrayString titles_columns;

    void SetTitlesColumn();

    void SetAutoSizeColumns();
};
