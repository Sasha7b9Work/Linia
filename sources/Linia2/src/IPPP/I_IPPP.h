// 2025/09/07 16:35:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include <memory>


// Здесь логика работы прибора (логика работы железа фактически)

class I_IPPP
{
public:

    virtual ~I_IPPP() = default;

    static void SetInstance(std::unique_ptr<I_IPPP>);

    static I_IPPP *GetInstance();

    // Вызывается в главном цикле
    virtual void PeriodicTask() = 0;

    // Вызывается при нажатии кнопки СТАРТ
    virtual void PressButtonStart() = 0;

    // Вызывается при нажатии кнопки СТОП
    virtual void PressButtonStop() = 0;

protected:

    // Читает четыре массива значений из ПЛИС
    virtual bool ReadData(int data_dac[NUMBER_ADC][POINTS_IN_SAMPLE_ADC], int data_code[POINTS_IN_SAMPLE_ADC]) = 0;

private:

    static std::unique_ptr<I_IPPP> instance;
};
