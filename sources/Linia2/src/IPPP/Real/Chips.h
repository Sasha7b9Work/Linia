// 2025/10/17 11:39:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


// Эта структура описывает регистр ПЛИС, регистр или ЦАП блока
struct Chip
{
    virtual ~Chip() { }

    static void Init();

    // Под этим именем она обозначена в плате контроллера
    virtual wxString GetNameDevice() const = 0;

    // Ширина в битах
    virtual int BitDepth() const = 0;

    // Послать значение в контроллер
    void WriteValue(uint) const;

    // Послать в контроллер размер регистра
    void WriteWidthToDevice() const;

    virtual bool IsDAC() const { return false; }
};


struct DAC : public Chip
{
    enum E
    {
        _0_ChannelC_Form,   // Формирователь развёртки       XP7 Записывается прямо из Orange Pi 5 Plus
        _1_ChannelC_Meas,   // Измеритель                    XP7
        _2_ChannelB_DAC1,   // База DAC1                     XP4
        _3_ChannelB_DAC2,   // База DAC2                     XP4
        _4_ChannelS_DAC1,   //                               XP5
        _5_ChannelS_DAC2,   //                               XP5
        _6_Source_50V,      // Источник 50 В                 XP10 Записывается прямо из Orange Pi 5 Plus
        Count
    };

    DAC(E i);

    virtual wxString GetNameDevice() const override;

    virtual int BitDepth() const override;

    virtual bool IsDAC() const override
    {
        return true;
    }

    E GetType() const
    {
        return v;
    }

private:

    E v = Count;
};


struct REG : public Chip
{
    enum E
    {
        _0_Source3kV,       // XP13
        _1_Commutator,      // XP12
        _2_ChannelC,        // XP7
        _3_ChannelB,        // XP4
        _4_ChannelS,        // XP5
        _5_MeasCurrent,     // XP6
        Count
    };

    REG(E i);

    virtual wxString GetNameDevice() const override;

    virtual int BitDepth() const override;

private:

    E v = Count;
};


struct FPGA : public Chip
{
    enum E
    {
        _0,
        _1,
        _2,
        Count
    };

    FPGA(E i);

    virtual wxString GetNameDevice() const override;

    virtual int BitDepth() const override;

private:

    E v = Count;
};


extern DAC *dacs[DAC::Count];
extern REG *regs[REG::Count];
extern FPGA *fpgas[FPGA::Count];
