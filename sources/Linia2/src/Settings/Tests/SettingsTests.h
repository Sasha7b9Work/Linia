// 2025/08/29 18:39:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


class wxArrayString;


// Категория испытуемого прибора
struct TypeCategory
{
    enum E
    {
        BJT,            // Биполярный транзистор
        BJT4,           // Биполярный транзистор с четвёртым выводом
        JFET,           // Полевой транзистор
        JFET4,          // Полевой транзисото с четвёртым выводм
        Thyristor,
        Diod,
        Resistor,
        Capacitor,
        Count
    };
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

    pchar Name() const;

    bool IsBS() const
    {
        return value == _B || value == _S;
    }
};


extern const Chan ChC;
extern const Chan ChB;
extern const Chan ChS;


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
