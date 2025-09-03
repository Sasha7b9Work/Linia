#include "defines.h"
#include "Communicator/GPIO/GPIO.h"

PinIn pinSTART(Pin::START);
PinIn pinSTOP(Pin::STOP);
PinIn pinDAT_F0(Pin::DAT_F0);
PinIn pinDAT_F1(Pin::DAT_F1);
PinIn pinDAT_F2(Pin::DAT_F2);
PinIn pinDAT_F3(Pin::DAT_F3);
PinIn pinREQ_RD(Pin::REQ_RD);
PinIn pinFIFO_FULL(Pin::FIFO_FULL);
PinIn pinFIFO_EMPTY(Pin::FIFO_EMPTY);


PinOut pinOUT1(Pin::OUT_1);
PinOut pinOUT2(Pin::OUT_2);
PinOut pinOUT3(Pin::OUT_3);
PinOut pinOUT4(Pin::OUT_4);
PinOut pinOUT5(Pin::OUT_5);
PinOut pinOUT6(Pin::OUT_6);
PinOut pinOUT7(Pin::OUT_7);
PinOut pinOUT8(Pin::OUT_8);
PinOut pinOUT9(Pin::OUT_9);


#ifdef ARM64

void GPIO::Init()
{

}

bool Pin::Get() const
{
    return false;
}

void Pin::Set(bool)
{

}

void Pin::SetChangeCallback(ChangeCallback callback)
{
    onChange = callback;
}


void 


#else

void GPIO::Init()
{

}

bool Pin::Get() const
{
    return false;
}

#endif