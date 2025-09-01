// 2025/08/29 18:39:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Категория испытуемого прибора
struct Category
{
    enum E
    {
        Diod,
        Thyristor,
        BCE_N,          // Биполярный NPN-транзистор
        BCE_P,          // Биполярный PNP-транзистор
        GDS_N,          // Полевой NMOS-транзистор
        GDS_P,          // Полевой PMOS-транзистор
        BCSE_N,         // Биполярный NPN-транзистор четырёхполюсный
        BCSE_P,         // Биполярный PNP-транзистор четырёхполюсный
        GDBS_N,         // Полевой NMOS-транзистор четырёхполюсный
        GDBS_P,         // Полевой PMOS-транзистор четырёхполюсный
        Count
    };

    static E Current();
};


#define ChC Chan::_C
#define ChB Chan::_B
#define ChS Chan::_S
#define ChE Chan::_E


struct Chan
{
    enum E
    {
        _C,      // Коллектор
        _B,      // База
        _S,      // Подложка
        _E,      // Эмиттер - общий
        Count
    };

    Chan(E v) : value(v)
    {
    }

    E value;

    bool IsVisible() const;
};


struct StateJack
{
    enum E
    {
        _C,
        _B,
        _S,
        _E,
        General,        // Общий
        General_1k,     // Общий 1к
        Break,          // Обрыв
        Count
    };

    static void PrepareArray(wxArrayString &, E, E, E = Count, E = Count);

    static wxString Name(E);
};


// Тип коммутации       CLineDlg::ShowGnezdoKommutator()
struct TypeCommutation
{
    enum E
    {
        Int,            // Внутренняя
        Ext,            // Внешняя
        Count
    };

    static bool IsInternal();
};


// Режим источника
struct ModeSource
{
    enum E
    {
        U,
        I,
        Count
    };
};


// Режим измерителя
struct ModeMeas
{
    enum E
    {
        U,
        I,
        Count
    };
};


// Тип развёртки
struct TypeScan
{
    enum E
    {
        ImpulsePos,
        ImpulseNeg,
        DCPos,
        DCNeg,
        SYNPos,
        SYNNeg,
        AC,
        Count
    };
};


// Первая очередь
struct FirstQueue
{
    enum E
    {
        ChannelC,
        ChannelB,
        Count
    };
};


// Семейство кривых
struct GenerationStup
{
    enum E
    {
        Disable,
        Enable,
        Count
    };
};
