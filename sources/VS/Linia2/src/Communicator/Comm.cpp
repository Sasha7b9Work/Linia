// 2025/6/11 18:47:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/Comm.h"
#include "Communicator/ComPort/ComPort.h"
#include "Communicator/SPI/SPI.h"


void Comm::Init()
{
#ifdef WIN32

    ComPort::Update();

#else

    SPI::Init();

#endif
}
