// 2025/09/11 21:22:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


/*
    Энкодер A - 11
    Энкодер B - 13
    Старт     - 15
    Стоп      - 33
*/


namespace Keyboard
{
    void Init(void (*funcOnKeyStart)(bool), void (*funcOnKeyStop)(bool), void (*funcOnEncoder)(int));

    void DeInit();
}
