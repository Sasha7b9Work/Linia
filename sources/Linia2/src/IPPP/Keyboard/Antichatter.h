// 2026/09/01 10:09:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


// Обработка антидребезга
//

class Antichatter
{
public:

    // В конструктор передаётся значение ожидания завершения дребезга в миллисекундах
    Antichatter(int timeAntichatterMS);

    void Input(bool, int64 timeMS);

    bool Output(int16 timeMS);

private:

    int timeAntichatterMS = 0;
};
