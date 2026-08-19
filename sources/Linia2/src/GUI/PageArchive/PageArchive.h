// 2026/04/08 15:03:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/Controls/Notebook.h"


class PageArchive : public PageNotebook
{
public:

    PageArchive(Notebook *, PageArchive *&);
};
