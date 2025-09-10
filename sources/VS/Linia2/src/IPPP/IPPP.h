// 2025/09/07 16:35:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Здесь логика работы прибора (логика работы с контроллером фактически)
class IPPP
{
public:

    virtual void Init() = 0;

    // Вызывается в главном цикле
    virtual void Update() = 0;

    // Вызывается при нажатии кнопки СТАРТ
    virtual void PressButtonStart() = 0;

    // Вызывается при нажатии кнопки СТОП
    virtual void PressButtonStop() = 0;

    static IPPP *self;

protected:

    virtual ~IPPP() { }

    // Читает четыре массива значений из ПЛИС
    virtual bool ReadData(std::vector<int>(&data)[4]) = 0;
};
