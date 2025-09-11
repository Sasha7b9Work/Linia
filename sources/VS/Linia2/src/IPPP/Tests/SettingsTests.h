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
    static wxString NameWx(E);
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


// Диапазон амплитуды
struct AmplitudeRange
{
    enum E
    {
        Range_200mV = 0,
        Range_2V = 1,
        Range_20V = 2,
        Range_200V = 3,
        Count
    };

    static wxString Name(E);
    static uint8_t Value(E);
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


// Количество точек измерения
struct PointCount
{
    enum E
    {
        Points_20,
        Points_50,
        Count
    };

    static wxString Name(E);
};


// Тип запуска
struct StartTrigger
{
    enum E
    {
        Internal,
        External,
        Count
    };

    static wxString Name(E);
};


// Режим запуска
struct StartMode
{
    enum E
    {
        Single,
        Multiple,
        Count
    };

    static wxString Name(E);
};


// Режим измерения
struct MeasMode
{
    enum E
    {
        Manual,
        Auto_1,
        Auto_2,
        Auto_3,
        Auto_4,
        Auto_5,
        Count
    };

    static wxString Name(E);
};


// Код DAC
struct DacCode
{
    enum E
    {
        Code_100,
        Code_110,
        Code_120,
        Code_130,
        Code_140,
        Code_150,
        Count
    };

    static wxString Name(E);
    static uint8_t Value(E);
};


// Состояние импульса
struct PulseState
{
    enum E
    {
        Disabled,
        Enabled,
        Count
    };

    static wxString Name(E);
};


// Тип элемента
struct ElementType
{
    enum E
    {
        TwoPole,
        Multipole,
        Count
    };

    static wxString Name(E);
};


// Результат ограничения
struct LimitResult
{
    enum E
    {
        Pass,
        Fail,
        NotReached,
        Protection,
        Count
    };

    static wxString Name(E);
};


// Номинальная мощность
struct PowerNominal
{
    enum E
    {
        Nominal_110V,
        Nominal_220V,
        Count
    };

    static wxString Name(E);
};


// Длительность импульса
struct PulseDuration
{
    enum E
    {
        Duration_50us,
        Duration_100us,
        Duration_200us,
        Duration_300us,
        Duration_400us,
        Duration_500us,
        Duration_1000us,
        Count
    };

    static wxString Name(E);
    static uint Value(E);
};


// Процент ограничения
struct LimitPercent
{
    enum E
    {
        Percent_0 = 0,
        Percent_5 = 1,
        Percent_10 = 2,
        Percent_15 = 3,
        Percent_20 = 4,
        Percent_25 = 5,
        Percent_30 = 6,
        Percent_35 = 7,
        Percent_40 = 8,
        Percent_45 = 9,
        Percent_50 = 10,
        Percent_55 = 11,
        Percent_60 = 12,
        Percent_65 = 13,
        Percent_70 = 14,
        Percent_75 = 15,
        Percent_80 = 16,
        Percent_85 = 17,
        Percent_90 = 18,
        Percent_95 = 19,
        Percent_100 = 20,
        Count
    };

    static wxString Name(E);
    static int Value(E);
};
