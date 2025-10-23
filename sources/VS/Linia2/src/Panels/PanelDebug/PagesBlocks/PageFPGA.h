// 2025/10/13 13:53:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class PageFPGA : public wxPanel
{
public:

    PageFPGA(wxNotebook *);

    static PageFPGA *self;

    void Pack();
    void Unpack();
};
