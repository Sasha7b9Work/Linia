// 2025/10/17 11:39:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Device/Chips.h"


DAC *dacs[DAC::Count];
REG *regs[REG::Count];
FPGA *fpgas[FPGA::Count];


void Chip::Init()
{
    for (int i = 0; i < DAC::Count; i++)
    {
        dacs[i] = new DAC((DAC::E)i);
    }

    for (int i = 0; i < REG::Count; i++)
    {
        regs[i] = new REG((REG::E)i);
    }

    for (int i = 0; i < FPGA::Count; i++)
    {
        fpgas[i] = new FPGA((FPGA::E)i);
    }
}


DAC::DAC(E _v) : v(_v)
{

}


wxString DAC::GetNameSTM32() const
{
    return wxString::Format("DAC%d", (int)v);
}


REG::REG(E _v) : v(_v)
{

}


wxString REG::GetNameSTM32() const
{
    return wxString::Format("REG%d", (int)v);
}


FPGA::FPGA(E _v) : v(_v)
{

}


wxString FPGA::GetNameSTM32() const
{
    return wxString::Format("FPGA%d", (int)v);
}
