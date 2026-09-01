// 2026/09/01 10:09:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


// Обработка антидребезга

class Antichatter
{
public:

    // В конструктор передаётся значение ожидания завершения дребезга в миллисекундах
    Antichatter(int timeAntichatterMS);



private:

    int timeAntichatterMS = 0;
};
