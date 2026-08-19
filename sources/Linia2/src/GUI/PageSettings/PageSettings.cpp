// 2026/04/08 15:15:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "GUI/PageSettings/PageSettings.h"


PageSettings *ThePanelSettings = nullptr;


PageSettings::PageSettings(Notebook *board, PageSettings *&self) :
    PageNotebook(board, L("Настройки"))
{
    self = this;
}
