// 2025/08/29 18:39:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


class wxArrayString;


// Категория испытуемого прибора
struct TypeCategory
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

    explicit Chan(E v) : value(v) { }

    E value;

    bool IsVisible() const;

    pchar Name() const;

    bool IsBS() const
    {
        return value == _B || value == _S;
    }
};


extern const Chan ChC;
extern const Chan ChB;
extern const Chan ChS;


// Режим измерителя
struct ModeMeas
{
    enum E
    {
        U,
        I,
        Count
    };

    static pchar Name(E);
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

    static pchar NameShort(E);

    static pchar _NameGUI(E);

    static pchar NameFileICO(E);
};
