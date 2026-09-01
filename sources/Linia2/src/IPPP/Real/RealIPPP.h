// 2025/09/10 21:27:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "IPPP/I_IPPP.h"
#include "Settings/Tests/SettingsTests.h"
#include <iostream>
#include <fstream>


class RealIPPP : public I_IPPP
{
public:

    virtual ~RealIPPP() override {}

    void ApplicationTask() override;

private:

    virtual bool ReadData(int data_dac[NUMBER_ADC][POINTS_IN_SAMPLE_ADC], int data_code[POINTS_IN_SAMPLE_ADC]) override;

    // Бинарный файл
    std::ofstream binaryFile;
    bool binaryFileOpened = false;

    // Текстовый файл
    std::ofstream textFile;
    bool textFileOpened = false;

    wxString currentBinaryFileName;
    wxString currentTextFileName;

    void OpenNewBinaryFile();
    void CloseBinaryFile();
    void OpenNewTextFile();
    void CloseTextFile();
    void WriteDataToTextFile(int16 data[5]);
};
