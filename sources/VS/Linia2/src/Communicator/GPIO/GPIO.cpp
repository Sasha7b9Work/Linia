#include "defines.h"
#include "Communicator/GPIO/GPIO.h"


#ifdef ARM64

extern Pin pinSTART(Pin::START);
extern Pin pinSTOP(Pin::STOP);
extern Pin pinDAT_F0(Pin::DAT_F0);
extern Pin pinDAT_F1(Pin::DAT_F1);
extern Pin pinDAT_F2(Pin::DAT_F2);
extern Pin pinDAT_F3(Pin::DAT_F3);
extern Pin pinREQ_RD(Pin::REQ_RD);
extern Pin pinFIFO_FULL(Pin::FIFO_FULL);
extern Pin pinFIFO_EMPTY(Pin::FIFO_EMPTY);


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
#endif