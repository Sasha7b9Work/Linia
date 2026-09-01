// 2025/09/11 08:48:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "IPPP/IDevice.h"
#include "Utils/BackgroundWorker.h"
#include <atomic>
#include <iostream>
#include <fstream>


class RealDevice : public IDevice
{
public:

    RealDevice() = default;

    virtual ~RealDevice();

    virtual bool Init() override;

    virtual void DeInit() override;

    virtual void ApplicationTask() override;

    virtual bool IsConnected() const override;

    virtual void SendCommand(pchar format, ...) const override;

    virtual bool ReadData(int data_dac[NUMBER_ADC][POINTS_IN_SAMPLE_ADC], int data_code[POINTS_IN_SAMPLE_ADC]) override;

private:

    std::atomic<bool> running = false;
    std::atomic<bool> connected = false;

    static void FuncOnKeyStart(bool);

    static void FuncOnKeyStop(bool);

    static void FuncOnEncoder(int);

    static void BackgroundTask();
    static BackgroundWorker worker;

    // Бинарный файл
    std::ofstream binaryFile;

    // Текстовый файл
    std::ofstream textFile;

    wxString currentBinaryFileName;
    wxString currentTextFileName;

    void OpenNewBinaryFile();
    void CloseBinaryFile();
    void OpenNewTextFile();
    void CloseTextFile();
    void WriteDataToTextFile(int16 data[5]);
};
