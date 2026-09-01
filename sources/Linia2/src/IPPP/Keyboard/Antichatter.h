// 2026/09/01 10:09:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


// Обработка антидребезга

// !!! ВНИМАНИЕ !!! время в вызове какого-либо метода не может быть меньше, чем в предыдущем вызове любого метода

class Antichatter
{
public:

    // В конструктор передаётся значение ожидания завершения дребезга в миллисекундах
    Antichatter(int _timeAntichatterMS, void (* _funcOnChange)(bool)) :
        funcOnChnage(_funcOnChange),
        timeAntichatterMS{ _timeAntichatterMS }
    {
    }

    bool InOut(bool, int64 timeMS);

    bool GetState() const
    {
        return prev_state;
    }

    void (*funcOnChnage)(bool);        // Эта функция вызывается при изменении состояния

private:

    const int64 timeAntichatterMS = 0;  // Время антидребезга

    bool prev_state = false;            // Предыдущее состояние
    int64 prev_time_change = 0;         // Время предыдущего изменения состояния
    int64 prev_time_input = 0;          // Время предудущего обращения
};
