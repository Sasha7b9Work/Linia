// 2026/04/08 15:06:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "GUI/PageArchive/PageArchive.h"


PageArchive *ThePanelArchive = nullptr;


PageArchive::PageArchive(Notebook *board, PageArchive *&global) : PageNotebook(board, L("Архив"))
{
    global = this;
}
