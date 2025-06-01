// 2024/7/18 14:34:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Indicator
{
    // Этим цветом индикатор будет гореть, когда подключится к ПО
    void On();

    // Этим цветом индикатор будет горить, когда ПО отключится
    void Off();

    // Этот цвет, когда карта лежит
    void Card();

    // Изменить цвет на очередное элементарное действие
    void Task();
}
