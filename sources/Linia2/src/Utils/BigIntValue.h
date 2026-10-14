// 2026/10/14 10:15:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


// Структра для представления чисел без потери точности
struct BigIntValue
{
    struct Order
    {
        enum E
        {
            pico,   // -12
            nano,   // -9
            micro,  // -6
            mini,   // -3
            zero,   //  0
            Kilo,   //  3
            Mega,   //  6
            Count
        };

        Order(E v) : value(v) { }

        void operator++(int)
        {
            int new_value = ((int)value) + 1;
            value = (E)new_value;
        }

    private:

        E value;
    };

    BigIntValue(int64 value, Order order);
};
