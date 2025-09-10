// 2025/09/07 16:35:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Здесь логика работы прибора (логика работы с контроллером фактически)
class IPPP
{
public:

    virtual void Init() = 0;

    virtual void Start() = 0;

    virtual void Stop() = 0;

    // Читает четыре массива значений из ПЛИС
    virtual bool ReadData(std::vector<int>(&data)[4]) = 0;

    static IPPP *self;

protected:

    virtual ~IPPP() { }
};
