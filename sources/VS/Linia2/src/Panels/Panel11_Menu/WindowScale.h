// 2025/7/25 10:02:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class WindowScale : public wxDialog
{
public:

    static const int WIDTH = 500;
    static const int HEIGHT = 500;

    WindowScale();

private:

    wxStaticBox *CreateBox(int x, int y, int w, int h, pchar axe, pchar units, int id_min, int id_max);
};
