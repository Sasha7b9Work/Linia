// 2025/10/07 11:47:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class PageCommutator : public wxPanel
{
public:

    PageCommutator(wxNotebook *);

    static PageCommutator *self;
};
