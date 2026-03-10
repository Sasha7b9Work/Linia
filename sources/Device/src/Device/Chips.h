// 2025/10/15 16:05:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL_PINS.h"


/*
*   Здесь всё, что относится к управлению регистрами и ЦАП-ми всех блоков
*/


struct Chip
{
    Chip(uint _l, PinOut *_cs, PinOut *_clk, PinOut *_dat) : cs(_cs), clk(_clk), dat(_dat), length(_l) { }

    void SetLength(uint _length)
    {
        length = _length;
    }

protected:

    PinOut *cs;
    PinOut *clk;
    PinOut *dat;
    uint length = 0;
    uint buffer_value = 0;         // Здесь хранится буферное значение, которое по команде Start() переписываетя в аппаратный регистр на плате
    bool is_running = false;
};


struct ChipDAC : public Chip
{
    enum E
    {
        CHAN_C_PCM,
        CHAN_C_RANGE,
        CHAN_B_1,
        CHAN_B_2,
        CHAN_S_1,
        CHAN_S_2,
        SOURCE_50V_PCM,
        Count
    };

    ChipDAC(E v, uint _l, PinOut *_cs, PinOut *_clk, PinOut *_dat) : Chip(_l, _cs, _clk, _dat), type(v)  { }

    static ChipDAC &Get(E);

    // Запись значения в буферный регистр. Перезапись в регистр устойства происходит по
    // команде Start()
    void WriteValue(uint);

    // По этой команде происходит запись нулевого значения в регистр на плате
    void Stop();

    // По этой команде происходит запись значения из буферного регистра в аппаратный регист на плате
    void Start();

private:

    E type;

    static ChipDAC dacs[10];

    // Непросредственная запись значения в железный регистр на плате
    void WriteValueRAW(uint);
};


struct ChipREG : public Chip
{
    enum E
    {
        SOURCE_3kV,
        COMMUTATOR,
        CHAN_C,
        CHAN_B,
        CHAN_S,
        MEAS_I,
        Count
    };

    ChipREG(E v, uint _l, PinOut *_cs, PinOut *_clk, PinOut *_dat) :
        Chip(_l, _cs, _clk, _dat), type(v) { }

    static ChipREG &Get(E);

    // Запись значения в буферный регистр. Перезапись в регистр устойства происходит по
    // команде Start()
    void WriteValue(uint);

    // По этой команде происходит запись нулевого значения в регистр на плате
    void Stop();

    // По этой команде происходит запись значения из буферного регистра в аппаратный регист на плате
    void Start();

private:

    E type;

    static ChipREG regs[10];

    // Непросредственная запись значения в железный регистр на плате
    void WriteValueRAW(uint);
};
