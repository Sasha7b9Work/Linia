// 2025/09/11 21:22:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Device/Keyboard.h"
#include "Communicator/GPIO/GPIO.h"
#include "Utils/Timer.h"
#include "Application.h"


namespace Keyboard
{
    static const int64 TIME_EVENT_BTN = 100;    // Столько мс состояние кнопки не должно меняться, чтобы действие свершилось (пропустить дребезг контактов)
    static const int64 TIME_EVENT_GOV = 3;

    static void CallbackOnSTART(bool);
    static void CallbackOnSTOP(bool);
    static void CallbackOnKA(bool);
    static void CallbackOnKB(bool);

    struct StructPin
    {
        PinIn *pin = nullptr;
        int64  event_time = 0;     // Время предыдущего изменения состояния. Если 0, то изменения не было
        bool   press = false;      // Предыдущее состояние. true - была нажата (переход из 0 в 1), false - была отпущена (переход из 1 в 0)
    };

    static StructPin pins[4];
}


void Keyboard::Init()
{
    pinSTART.SetChangeCallback(CallbackOnSTART);

    pinSTOP.SetChangeCallback(CallbackOnSTOP);

    pinKA.SetChangeCallback(CallbackOnKA);

    pinKB.SetChangeCallback(CallbackOnKB);

    pins[0].pin = &pinSTART;
    pins[1].pin = &pinSTOP;
    pins[2].pin = &pinKA;
    pins[3].pin = &pinKB;
}


void Keyboard::Update()
{
    int64 time = Timer::CurrentTimeMS();

    if (pins[0].event_time)                                     // Идёт событие - нажатие или отпускание
    {
        if (time - pins[0].event_time > TIME_EVENT_BTN)         // Если после последнего события прошло достаточно времени
        {
            Application::self->OnButtonStart(pins[0].press);    // То считаем, что кнопка в устойчивом положении - обрабатываем нажатие
            pins[0].event_time = 0;                             // И устанавливаем признак того, что событие произошло
        }
    }

    if (pins[1].event_time)
    {
        if (time - pins[1].event_time > TIME_EVENT_BTN)
        {
            Application::self->OnButtonStop(pins[1].press);
            pins[1].event_time = 0;
        }
    }

    if (pins[2].event_time && pins[3].event_time)
    {
        if ((time - pins[2].event_time) > TIME_EVENT_GOV &&
            (time - pins[3].event_time) > TIME_EVENT_GOV)
        {
            if (pins[2].press && !pins[3].press)
            {
                Application::self->OnGovernor(pins[2].event_time > pins[3].event_time);
            }
            else if (!pins[2].press && pins[3].press)
            {
                Application::self->OnGovernor(pins[2].event_time > pins[3].event_time);
            }

            pins[2].event_time = 0;
            pins[3].event_time = 0;
        }
    }
}


void Keyboard::CallbackOnSTART(bool press)
{
    pins[0].press = press;
    pins[0].event_time = Timer::CurrentTimeMS();
}


void Keyboard::CallbackOnSTOP(bool press)
{
    pins[1].press = press;
    pins[1].event_time = Timer::CurrentTimeMS();
}


void Keyboard::CallbackOnKA(bool press)
{
    pins[2].press = press;
    pins[2].event_time = Timer::CurrentTimeMS();
}


void Keyboard::CallbackOnKB(bool press)
{
    pins[3].press = press;
    pins[3].event_time = Timer::CurrentTimeMS();
}
