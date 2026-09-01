// 2025/09/11 21:22:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "IPPP/Keyboard/Keyboard.h"
#include "Communicator/GPIO/GPIO.h"
#include "Utils/Timer.h"
#include "IPPP/Keyboard/Antichatter.h"
#include "Application.h"
#include "Utils/BackgroundWorker.h"
#include <iterator> 


namespace Keyboard
{
    static void BackgroundTask();

    BackgroundWorker worker{ BackgroundTask, 1 };

    static void EmptyFuncBool(bool)
    {
        LOG_ERROR("Function \"%s\" not initialized", __FUNCTION__);
    }

    static void EmptyFuncInt(int)
    {
        LOG_ERROR("Function \"%s\" not initialized", __FUNCTION__);
    }

    static void (*FuncOnEncoder)(int) = EmptyFuncInt;
    static void FuncOnEncAB(bool);

#define PIN_START pins[0]
#define PIN_STOP  pins[1]
#define PIN_ENC_A pins[2]
#define PIN_ENC_B pins[3]

    struct StructPin
    {
        PinIn *pin = nullptr;
        Antichatter antichatter;
    };

    static StructPin pins[4] =
    {
        { &pinSTART, { 1000, EmptyFuncBool } },
        { &pinSTOP, { 1000, EmptyFuncBool } },
        { &pinEncA, { 3, FuncOnEncAB } },
        { &pinEncB, { 3, FuncOnEncAB } }
    };
}


void Keyboard::Init(void (*funcOnKeyStart)(bool), void (*funcOnKeyStop)(bool), void (*funcOnEncoder)(int))
{
    PIN_START.antichatter.funcOnChnage = funcOnKeyStart;
    PIN_STOP.antichatter.funcOnChnage = funcOnKeyStop;

    FuncOnEncoder = funcOnEncoder;

    worker.Start();
}


void Keyboard::BackgroundTask()
{
    int64 time = Timer::CurrentTimeMS();

    for (size_t i = 0; i < std::size(pins); i++)
    {
        StructPin &pin = pins[i];

        pin.antichatter.InOut(pin.pin->GetState(), time);
    }
}


void Keyboard::FuncOnEncAB(bool)
{
    if (PIN_ENC_A.antichatter.GetState() != PIN_ENC_B.antichatter.GetState())
    {
        if (PIN_ENC_A.antichatter.GetState() && !PIN_ENC_B.antichatter.GetState())
        {
            FuncOnEncoder(1);
        }
        else if (!PIN_ENC_A.antichatter.GetState() && PIN_ENC_B.antichatter.GetState())
        {
            FuncOnEncoder(-1);
        }
    }
}


void Keyboard::DeInit()
{
    worker.Stop();
}
