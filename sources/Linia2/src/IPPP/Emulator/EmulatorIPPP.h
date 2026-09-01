// 2025/09/10 21:36:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "IPPP/I_IPPP.h"


class EmulatorIPPP : public I_IPPP
{
public:

    virtual ~EmulatorIPPP() override {}

    void PeriodicTask() override;

private:

    bool in_process_measuring = false;                  // Устанавливается при нажатии кнопки СТАРТ. Снимается при нажатии кнопки СТОП.

    virtual bool ReadData(int data_dac[NUMBER_ADC][POINTS_IN_SAMPLE_ADC], int data_code[POINTS_IN_SAMPLE_ADC]) override;
};
