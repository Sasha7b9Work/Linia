// 2026/09/29 11:03:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#pragma warning(push, 0)
    #include <wx/dcbuffer.h>
#pragma warning(pop)


// Object Subject to Testing - ОПИ - объект, подлежащий исследованию


class OStT
{
public:

    virtual void Draw(wxAutoBufferedPaintDC &) = 0;

protected:

    OStT();
};


// ОПИ с двумя выводами


class OStT2 : public OStT
{
public:
private:
};


// ОПИ с тремя выводами


class OStT3 : public OStT
{
public:
    OStT3();
private:
};


// ОПИ с четырьмя выводами


class OStT4 : public OStT
{
public:
private:
};
