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
        Darlington,
        Thyristor,
        Diod,
        Resistor,
        Capacitor,
        Count
    };
};


// Тип биполярного транзистора и транзистора Дарлингтона
struct TypeBJT
{
    enum E
    {
        NPN,
        PNP,
        count
    };

    TypeBJT(E e) : type{ e } { }

    bool IsNPN() const
    {
        return type == NPN;
    }

    bool IsPNP() const
    {
        return type == PNP;
    }

    void Set(E e)
    {
        type = e;
    }

private:

    E type;
};


// Тип полевого транзистора
struct TypeFET
{
    enum E
    {
        ChannelP,
        ChannelN,
        Count
    };

    TypeFET(E e) : type{ e } { }

    bool IsChannelP() const
    {
        return type == ChannelP;
    }

    bool IsChannelN() const
    {
        return type == ChannelN;
    }

    void Set(E e)
    {
        type = e;
    }

private:

    E type;
};


// Подключение - с общим анодом или катодом
struct CommonElectrode
{
    enum E
    {
        Anode_P,     // К земле (эмиттеру) подключён анод (треугольник)
        Catode_N,    // К земле (эмиттеру) подключён катод (чёрточка)
        Count
    };

    CommonElectrode(E e) : type{ e } { }

    bool IsAnode() const
    {
        return type == Anode_P;
    }

    bool IsCatode() const
    {
        return type == Catode_N;
    }

    void Set(E e)
    {
        type = e;
    }

private:

    E type;
};


// Управляющий электрод - катод или анод
struct ControlElectrode
{
    enum E
    {
        Anode_P,
        Catode_N,
        Count
    };

    ControlElectrode(E e) : type {e} { }

    bool IsAnode() const
    {
        return type == Anode_P;
    }

    bool IsCatode() const
    {
        return type == Catode_N;
    }

private:

    E type;
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

    static pchar NameGUI(E);

    static pchar NameFileICO(E);
};
