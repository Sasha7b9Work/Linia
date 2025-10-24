// 2025/6/1 18:08:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "Panels/PanelMenu/PanelMenu.h"


class PanelTable : public Panel
{
public:

    static const int WIDTH = MainWindow::WIDTH_DRAW;
    static const int HEIGHT = MainWindow::HEIGHT - MainWindow::HEIGHT_HI - PanelButtons::HEIGHT;

    PanelTable(wxWindow *parent);

    static PanelTable *self;

private:

    wxGrid *grid;

    wxArrayString titles_columns;

    struct Line
    {
        int p1;
        int p2;
        int p3;

        float Uc;
        float Ic;
        float Ib;
    };

    wxVector <Line> lines;

    void SetTitlesColumn();

    void SetAutoSizeColumns();

    void UpdateLines();

    void InsertValue(int row, int col, float value);
};
