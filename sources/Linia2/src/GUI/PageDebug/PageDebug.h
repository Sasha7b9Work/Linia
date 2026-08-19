// 2025/6/3 13:44:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/Controls/Notebook.h"


class PageDebug : public PageNotebook
{
public:

    static PageDebug *self;

    PageDebug(Notebook *);

    virtual bool Show(bool) override;

    void PeriodicTask();

    void Unpack();

    void Pack();
};
