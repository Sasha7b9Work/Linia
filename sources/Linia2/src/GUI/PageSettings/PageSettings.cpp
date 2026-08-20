// 2026/04/08 15:15:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageSettings/PageSettings.h"
#include "GUI/Controls/Sizers.h"


PageSettings *ThePageSettings = nullptr;


PageSettings::PageSettings(Notebook *board, PageSettings *&global) :
    PageNotebook(board, L("Настройки"))
{
    global = this;

    BoxSizerVert *sizer_main = new BoxSizerVert();

    BoxSizerHor *sizer1 = new BoxSizerHor();

    {
        btnFileManager = new Button(this, "Total Commander");
    }

    sizer_main->AddSizer(sizer1);
    sizer_main->AddStretchSpacer(1);

    SetSizer(sizer_main);
}
