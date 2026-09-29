// 2026/04/08 15:15:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageSettings/PageSettings.h"
#include "GUI/Controls/Sizers.h"


PageSettings *PageSettings::self = nullptr;


PageSettings::PageSettings(Notebook *board) :
    PageNotebook(board, L("Настройки"))
{
    self = this;

    BoxSizerVert *sizer_main = new BoxSizerVert();

    BoxSizerHor *sizer1 = new BoxSizerHor();

    {
        Button *btn = new Button(this, L("Мой компьютер"));
        sizer1->AddWidget(btn);
        btn->Bind(wxEVT_BUTTON, [](wxCommandEvent &)
            {

            });
    }

    {
        Button *btn = new Button(this, L("Калибровка"));
        sizer1->AddWidget(btn);
        btn->Bind(wxEVT_BUTTON, [](wxCommandEvent &)
            {

            });
    }

    sizer_main->AddSizer(sizer1);
    sizer_main->AddStretchSpacer(1);

    SetSizer(sizer_main);
}
