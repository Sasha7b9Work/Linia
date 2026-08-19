// 2026/04/08 15:06:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "GUI/PanelArchive/PanelArchive.h"


PanelArchive *ThePanelArchive = nullptr;


PanelArchive::PanelArchive(Notebook *board, PanelArchive *&global) : PageNotebook(board, L("Архив"))
{
    global = this;
}
