// 2026/04/08 15:06:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageArchive/PageArchive.h"


PageArchive *ThePageArchive = nullptr;


PageArchive::PageArchive(Notebook *board, PageArchive *&global) : PageNotebook(board, L("Архив"))
{
    global = this;
}
