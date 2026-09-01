#pragma once
#include "Communicator/GPIO/PinStorage.h"


class PinIn : public Pin
{
public:

    using Pin::Pin;

    using ChangeCallback = std::function<void(bool)>;

    // Время чтения пина - 0.5 мкс
    static bool GetHardware(gpiod_line *);

private:

    ChangeCallback callback_;
};


class PinOut : public Pin
{
public:

    using Pin::Pin;

    void Set(bool state);

    void ToLow();

    void ToHi();

    // Время установки пина - 0.5 мкс
    static void Set(gpiod_line *, int);
};

namespace GPIO
{
    void Init();
    void DeInit();

    PinInfo *GetPinInfo(Pin::E);
}

extern PinIn pinFIFO_FULL;      //   Значение "1" означает, что у ПЛИС на плате контроллера есть 

extern PinIn pinSTART;          //   Кнопка старт
extern PinIn pinSTOP;           //   Кнопка стоп
extern PinIn pinEncA;           // / Выводы
extern PinIn pinEncB;           // / энкодера
