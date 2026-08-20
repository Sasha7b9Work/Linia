// 2026/04/08 15:15:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageSettings/PageSettings.h"


PageSettings *ThePageSettings = nullptr;


PageSettings::PageSettings(Notebook *board, PageSettings *&global) :
    PageNotebook(board, L("Настройки"))
{
    global = this;
}
