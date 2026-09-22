// 2026/09/22 10:40:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Hardware/Bootloader.h"
#include <stm32f4xx_hal.h>

#ifdef WIN32
    #pragma warning(push)
    #pragma warning(disable:4312)
#endif


namespace Bootloader
{
    typedef void (*pFunction)(void);
}


void Bootloader::Run()
{
    uint bootloaderAddr = 0x080E0000;       // Адрес загрузчика
    uint JumpAddress;
    pFunction Jump_To_Application;

    // 1. ОТКЛЮЧАЕМ ГЛОБАЛЬНЫЕ ПРЕРЫВАНИЯ И SYSTICK
    // Это критически важно, чтобы прерывания из APP не сработали в Bootloader
    __disable_irq();
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;

    // 2. ОЧИЩАЕМ ВСЕ ЗАПРОСЫ ПРЕРЫВАНИЙ (NVIC)
    // Это обязательно, иначе "застрявшие" флаги могут вызвать ложные срабатывания
    for (int i = 0; i < 8; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF; // Отключить все прерывания
        NVIC->ICPR[i] = 0xFFFFFFFF; // Очистить все pending-флаги
    }

    // 3. УСТАНАВЛИВАЕМ НОВЫЙ УКАЗАТЕЛЬ СТЕКА (MSP)
    // Первое слово по адресу загрузчика — это начальный MSP
    __set_MSP(*(__IO uint32_t *)bootloaderAddr);

    // 4. ПОЛУЧАЕМ АДРЕС РЕЗЕТ-ВЕКТОРА (ТОЧКИ ВХОДА)
    // Второе слово — адрес Reset_Handler
    JumpAddress = *(__IO uint32_t *)(bootloaderAddr + 4);
    Jump_To_Application = (pFunction)JumpAddress;

    // 5. ВЫПОЛНЯЕМ ПЕРЕХОД
    Jump_To_Application();
}

#ifdef WIN32
    #pragma warning(pop)
#endif
