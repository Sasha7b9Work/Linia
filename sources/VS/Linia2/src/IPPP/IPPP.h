// 2025/09/07 16:35:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Здесь логика работы прибора (логика обмена с контроллером фактически)
namespace IPPP
{
    void Init();

    void Start();

    void Stop();

    // Читает четыре массива значений из ПЛИС
    bool ReadData(std::vector<int> (&data)[4]);
}
