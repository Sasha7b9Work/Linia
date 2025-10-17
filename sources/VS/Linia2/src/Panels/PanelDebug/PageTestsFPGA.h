// 2025/10/13 13:53:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class PageTestsFPGA : public wxPanel
{
public:

    PageTestsFPGA(wxNotebook *);

    static PageTestsFPGA *self;

    void Pack();
    void Unpack();
};
