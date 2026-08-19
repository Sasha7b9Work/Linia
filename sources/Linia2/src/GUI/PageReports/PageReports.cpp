// 2026/04/08 15:10:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageReports/PageReports.h"


PageReports *ThePanelReports = nullptr;


PageReports::PageReports(Notebook *board, PageReports *&self) :
    PageNotebook(board, L("Отчёты"))
{
    self = this;
}
