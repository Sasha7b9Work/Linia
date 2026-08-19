// 2026/04/08 15:17:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/Controls/Notebook.h"


class PageTables : public PageNotebook
{
public:

    PageTables(Notebook *, PageTables *&);
};
