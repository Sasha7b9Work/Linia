// 2025/08/19 14:10:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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


struct Channel
{
    enum E
    {
        _C,      // Коллектор
        _B,      // База
        _S,      // Подложка
        _E,      // Эмиттер - общий
        Count
    };

    Channel(E v) : value(v) { }

    E value;

    bool IsVisible() const;
};


class Test
{
    void Load(pchar file_name);

private:

    Category::E category;               // m_nelement           Вид тестируемого элемента:-1-не задано,0-D,1-R,2-NPN,3-PNP,4-NMOS,5-PMOS,6-NPN4,7-PNP4,8-NMOS4,9-PMOS4
    wxString remark;                    // m_PRIM               Комментарии (краткое описание теста)
                                        // m_iCircuitKol        0 - коллектор в обрыве, 1 - вкл
                                        // m_iCircuitBaza       1 - обрыв
                                        // m_iCircuitDop        0 - выкл
                                        // m_iKommutator        0 - внутренняя коммутация - прямое подключение электродов, 1 - внешняя коммутация
                                        // m_iGnezdoC           0, 1
                                        // m_iGnezdoB
                                        // m_iGnezdoE
                                        // m_irazv              // Канал С - развёртка, IMP_POS, DC_POS, SYN_POS, AC, SYN_NEG, DC_NEG, IMP_NEG
                                        // m_idlitimp           // Индекс длительности импульса
                                        // m_iKolU              // Индекс напряжения канала С
                                        // m_iKolI              // Индекс тока канала С
                                        // m_iKolSourceU = 0    // 
                                        // m_iLimitKolU = 50            текущее значение слайдера ограничения U коллектора (в %)
                                        // m_iLimitKolI = 0             текущее значение слайдера ограничения I коллектора
                                        // m_itipimp = 1                тип импульса TIP_SYN или TIP_IMP
                                        // m_iBazaSource = 1            0 - ист. U, 1 - ист. I
                                        // m_iAmplitudeStepBaza = 15
                                        // m_uStepBaza = 10
                                        // m_iPolarityStepBaza = 0
                                        // m_iRangBiasBaza = -1
                                        // m_iBiasBaza = 0              текущее значение слайдера смещения базы
                                        // m_iPolarityBiasBaza = 0
                                        // m_iBazaMeas = 1
                                        // m_iRangLimitBaza = 5
                                        // m_iLimitBaza = 1 0 0         текущее значение слайдера ограничения базы
                                        // m_iDopSource = -1
    int basa_step_alt = 0;              // m_iAmplitudeStepDop = -1     Индекс массива значений амплитуды ступеньки источника U (база, доп. канал)
                                        // m_uStepDop = 0
                                        // m_iPolarityStepDop = 0
                                        // m_iRangBiasDop = -1
                                        // m_iBiasDop = 0               текущее значение слайдера смещения доп.канала
                                        // m_iPolarityBiasDop = 0
                                        // m_iDopMeas = 0
                                        // m_iRangLimitDop = -1
                                        // m_iLimitDop = 100            текущее значение слайдера ограничения доп.канала
    wxString name_axis_Y = "Ib";        // m_sAxisY = Ib
    wxString name_axis_X = "Ic";        // m_sAxisX = Ic
    wxString s_par;                     // m_sPar= Beta                 Beta, Betai, R, S
                                        // m_sArg = Ic
                                        // m_uTp= 0
                                        // m_sUr1 = 20E-3
                                        // m_sUr2 =
                                        // m_iForm = -1
                                        // m_sMin =
                                        // m_sMax =
    bool checkColRejHigh = false;       // m_bCheckKolRejHigh = 0
    bool check_pause = false;           // m_bCheckPauza = 0
    bool basa_enable = false;           // m_bBazaVkl = 0                   признак: импульс в базе: 0 - sin (10ms), 1 - (0.2ms,0.5ms,2ms)
    bool check_baza_rej_high = false;   // m_bCheckBazaRejHigh = 0          амплитуду ступеньки базы уменьшить в 10 раз
};