// 2025/09/11 21:22:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "IPPP/Keyboard/Keyboard.h"
#include "Communicator/GPIO/GPIO.h"
#include "Utils/Timer.h"
#include "Application.h"
#include <iterator> 


namespace Keyboard
{
#define PIN_START pins[0]
#define PIN_STOP  pins[1]
#define PIN_ENC_A pins[2]
#define PIN_ENC_B pins[3]

    struct StructPin
    {
        int64  event_time = 0;          // Время предыдущего изменения состояния. Если 0, то изменения не было
        PinIn *pin = nullptr;
        bool   prev = false;            // Предыдущее состояние. true - была нажата (переход из 0 в 1), false - была отпущена (переход из 1 в 0)
        bool   current = false;         // Текущее состояние
        int16  time_antichatter = 0;    // Время антидребезга
    };

    static StructPin pins[4];

    void PeriodicTask();

    static void EmptyFuncBool(bool)
    {
        LOG_ERROR("Function \"%s\" not initialized", __FUNCTION__);
    }

    static void EmptyFuncInt(int)
    {
        LOG_ERROR("Function \"%s\" not initialized", __FUNCTION__);
    }

    static void (*FuncOnKeyStart)(bool) = EmptyFuncBool;
    static void (*FuncOnKeyStop)(bool) = EmptyFuncBool;
    static void (*FuncOnEncoder)(int) = EmptyFuncInt;
}


void Keyboard::PeriodicTask()
{
    int64 time = Timer::CurrentTimeMS();

    // PIN_START

    StructPin &pin = pins[0];

    if (pin.event_time)             // Ранее произошло событие, нужно проверить, истекло ли время переходных процессов
    {
        if (time - pin.event_time < pin.time_antichatter)
        {
            // Ничего не делаем - время защиты от антидребезга не истекло
        }
        else
        {
            if (pin.current != pin.prev)
            {

            }
        }
    }
}


void Keyboard::Init(void (*funcOnKeyStart)(bool), void (*funcOnKeyStop)(bool), void (*funcOnEncoder)(int))
{
    FuncOnKeyStart = funcOnKeyStart;
    FuncOnKeyStop = funcOnKeyStop;
    FuncOnEncoder = funcOnEncoder;

    pins[0] = { 0, &pinSTART, false, false, 100 };
    pins[1] = { 0, &pinSTOP, false, false, 100 };
    pins[2] = { 0, &pinEncA, false, false, 3 };
    pins[3] = { 0, &pinEncB, false, false, 3 };
}


void Keyboard::DeInit()
{

}


/*
void Keyboard::PeriodicTask()
{
    int64 time = Timer::CurrentTimeMS();

    if (PIN_START.event_time)                                     // Идёт событие - нажатие или отпускание
    {
        if (time - PIN_START.event_time > TIME_EVENT_BTN)         // Если после последнего события прошло достаточно времени
        {
//            TheApp->OnButtonStart(PIN_START.press);    // То считаем, что кнопка в устойчивом положении - обрабатываем нажатие
            PIN_START.event_time = 0;                             // И устанавливаем признак того, что событие произошло
        }
    }

    if (PIN_STOP.event_time)
    {
        if (time - PIN_STOP.event_time > TIME_EVENT_BTN)
        {
//            TheApp->OnButtonStop(PIN_STOP.press);
            PIN_STOP.event_time = 0;
        }
    }

    if (PIN_ENC_A.event_time && PIN_ENC_B.event_time)
    {
        if ((time - PIN_ENC_A.event_time) > TIME_EVENT_GOV &&
            (time - PIN_ENC_B.event_time) > TIME_EVENT_GOV)
        {
            if (PIN_ENC_A.press && !PIN_ENC_B.press)
            {
//                TheApp->OnGovernor(PIN_ENC_A.event_time > PIN_ENC_B.event_time);
            }
            else if (!PIN_ENC_A.press && PIN_ENC_B.press)
            {
//                TheApp->OnGovernor(PIN_ENC_A.event_time > PIN_ENC_B.event_time);
            }

            PIN_ENC_A.event_time = 0;
            PIN_ENC_B.event_time = 0;
        }
    }
}
*/
