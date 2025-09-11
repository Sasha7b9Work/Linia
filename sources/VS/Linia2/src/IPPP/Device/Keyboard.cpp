// 2025/09/11 21:22:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Device/Keyboard.h"
#include "Communicator/GPIO/GPIO.h"


namespace Keyboard
{
    static void CallbackOnSTART(bool);
    static void CallbackOnSTOP(bool);
    static void CallbackOnKA(bool);
    static void CallbackOnKB(bool);
}


void Keyboard::Init()
{
    pinSTART.SetChangeCallback(CallbackOnSTART);

    pinSTOP.SetChangeCallback(CallbackOnSTOP);

    pinKA.SetChangeCallback(CallbackOnKA);

    pinKB.SetChangeCallback(CallbackOnKB);
}


void Keyboard::Update()
{

}


void Keyboard::CallbackOnSTART(bool)
{

}


void Keyboard::CallbackOnSTOP(bool)
{

}


void Keyboard::CallbackOnKA(bool)
{

}


void Keyboard::CallbackOnKB(bool)
{

}
