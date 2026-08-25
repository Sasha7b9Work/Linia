// 2025/09/10 21:27:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "IPPP/I_IPPP.h"
#include "Settings/Tests/SettingsTests.h"


class RealIPPP : public I_IPPP
{
public:

    virtual ~RealIPPP() override {}

    void PeriodicTask() override;

    void PressButtonStart() override;

    void PressButtonStop() override;

private:

    bool IsChanBS(const Chan &) const;

    virtual bool ReadData(int data_dac[NUMBER_ADC][POINTS_IN_SAMPLE_ADC], int data_code[POINTS_IN_SAMPLE_ADC]) override;

    void Pause();
};
