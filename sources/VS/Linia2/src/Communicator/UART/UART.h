#pragma once


/*
    Используется для связи с платой контроллера
*/


#define UART_DEVICE   "/dev/ttyS6"
#define UART_BAUDRATE 921600
#define UART_MODE     "8N1"


namespace UART
{
    // Доступно ли в системе (только на ARM64)
    bool IsAvailability();

    bool Init(void (*recv_callback)(uint8));

    void ReInit();

    void DeInit();

    bool SendByte(uint8);

    // Отправка буфера данных через UART порциями по 32 байта
    bool SendBuffer(const void *buffer, int size);

    // \todo Эта функция не должна вызываться напрямую
    void Flush();

    namespace RecvCallback
    {
        // Сохранить текущую функцию обратного вызова на приём
        void Store();

        // Установить новую функцию обратного вызова на приём
        void Set(void (*callback)(uint8));

        // Восстановить ранее сохранённую функцию обратного вызова на приём
        void Restore();
    }
}
