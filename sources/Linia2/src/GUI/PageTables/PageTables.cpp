// 2026/04/08 15:17:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "GUI/PageTables/PageTables.h"


PageTables *ThePanelTables = nullptr;


PageTables::PageTables(Notebook *board, PageTables *&self) :
    PageNotebook(board, L("Таблицы"))
{
    self = this;
}
