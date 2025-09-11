// 2025/08/29 18:39:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Категория испытуемого прибора
struct Category
{
    enum E
    {
        BCE_N,          // Биполярный NPN-транзистор
        BCE_P,          // Биполярный PNP-транзистор
        GDS_N,          // Полевой NMOS-транзистор
        GDS_P,          // Полевой PMOS-транзистор
        BCSE_N,         // Биполярный NPN-транзистор четырёхполюсный
        BCSE_P,         // Биполярный PNP-транзистор четырёхполюсный
        GDBS_N,         // Полевой NMOS-транзистор четырёхполюсный
        GDBS_P,         // Полевой PMOS-транзистор четырёхполюсный
        Diod,
        Thyristor,
        Resistor,
        Capacitor,
        Count
    };

    static E Current();

    static bool IsBCE();        // Биполярный транзистор
    static bool IsGDS();        // Полевой транзистор
};


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

    wxString Name() const;
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

    // Эти обозначения используются в ГИП
    static wxString NameGUI(E);
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

    static wxString Name(E);
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

    static wxString Name(E);
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

    static wxString Name(E);
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

    static wxString Name(E);
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

    static wxString Name(E);
};
