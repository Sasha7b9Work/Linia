// 2026/04/08 15:17:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTables/PageTables.h"


PageTables *ThePageTables = nullptr;


PageTables::PageTables(Notebook *board, PageTables *&self) :
    PageNotebook(board, L("Таблицы"))
{
    self = this;
}
