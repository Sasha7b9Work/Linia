// 2025/09/10 21:28:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "IPPP/Real/RealIPPP.h"
#include "IPPP/IDevice.h"
#include "IPPP/Emulator/EmulatorDevice.h"
#include "Communicator/GPIO/GPIO.h"
#include "IPPP/Keyboard/Keyboard.h"
#include "IPPP/Real/RealDevice.h"
#include "Utils/Timer.h"
#include "Communicator/SPI/SPI.h"
#include "GUI/PageDebug/PanelRight.h"
#include <iomanip>


bool need_write_data_to_file = false;


bool RealIPPP::IsChanBS(const Chan &ch) const
{
    if (ch.IsBS())
    {
        return true;
    }

    LOG_ERROR("Channel must be B or S, not %s", ch.Name());

    return false;
}



void RealIPPP::PeriodicTask()
{
    Keyboard::PeriodicTask();

    IDevice::impl->PeriodicTask();

    int data_dac[NUMBER_ADC][POINTS_IN_SAMPLE_ADC];
    int data_code[POINTS_IN_SAMPLE_ADC];

    if (ReadData(data_dac, data_code))
    {
        for (int i = 0; i < NUMBER_ADC; i++)
        {
            ThePanelRight->data[i]->SetData(data_dac[i]);
        }

        ThePanelRight->data[NUMBER_ADC]->SetData(data_code);
    }
}


void RealIPPP::PressButtonStart()
{

}


void RealIPPP::PressButtonStop()
{

}


bool RealIPPP::ReadData(int data_dac[NUMBER_ADC][POINTS_IN_SAMPLE_ADC], int data_code[POINTS_IN_SAMPLE_ADC])
{
    bool result = false;

    static bool prev = false;

    if (pinFIFO_FULL.GetState() && prev == false)
    {
#ifndef _WIN32
        if (need_write_data_to_file)
        {
            OpenNewBinaryFile();
            OpenNewTextFile();
        }
#endif

        for (int i = 0; i < POINTS_IN_SAMPLE_ADC; i++)
        {
            uint16 data[5];

            SPI::ReadFPGA((uint8 *)data, NUMBER_ADC * 2 + 1);

#ifndef _WIN32
            if (need_write_data_to_file)
            {
                // Запись в бинарный файл
                if (binaryFileOpened && binaryFile.is_open())
                {
                    binaryFile.write((char *)data, NUMBER_ADC * 2 + 1);
                }

                // Запись в текстовый файл
                WriteDataToTextFile(data);
            }
#endif

            for (int num_dac = 0; num_dac < 4; num_dac++)
            {
                data_dac[num_dac][i] = data[num_dac];
            }

            data_code[i] = (uint8)data[4];
        }

#ifndef _WIN32
        if (need_write_data_to_file)
        {
            void CloseBinaryFile();
            void CloseTextFile();

            need_write_data_to_file = false;
        }
#endif

        result = true;
    }

    prev = pinFIFO_FULL.GetState();

    return result;
}


void RealIPPP::Pause()
{
    volatile int i = 0;

    for (i = 0; i < 1000; i += 1)
    {
    }
}


void RealIPPP::OpenNewBinaryFile()
{
    if (binaryFileOpened && binaryFile.is_open())
    {
        textFile.flush();
        binaryFile.close();
        binaryFileOpened = false;
    }

    wxDateTime now = wxDateTime::Now();
    wxString timestamp = now.Format("%Y%m%d_%H%M%S");

    wxString basePath = "/mnt/nvme/data/";
    currentBinaryFileName = basePath + timestamp + ".bin";

    binaryFile.open(currentBinaryFileName.ToStdString(), std::ios::binary);
    binaryFileOpened = binaryFile.is_open();

    if (binaryFileOpened)
    {
        LOG_WRITE("Created binary file: %s", currentBinaryFileName.c_str().AsChar());
    }
}

void RealIPPP::OpenNewTextFile()
{
    if (textFileOpened && textFile.is_open())
    {
        textFile.flush();
        textFile.close();
        textFileOpened = false;
    }

    wxDateTime now = wxDateTime::Now();
    wxString timestamp = now.Format("%Y%m%d_%H%M%S");

    wxString basePath = "/mnt/nvme/data/";
    currentTextFileName = basePath + timestamp + ".txt";

    textFile.open(currentTextFileName.ToStdString(), std::ios::out);
    textFileOpened = textFile.is_open();

    if (textFileOpened)
    {
        textFile << "    CH0    CH1    CH2    CH3   CODE\n";
        LOG_WRITE("Created text file: %s", currentTextFileName.c_str().AsChar());
    }
}

void RealIPPP::CloseBinaryFile()
{
    if (binaryFileOpened && binaryFile.is_open())
    {
        binaryFile.close();
        binaryFileOpened = false;
        LOG_WRITE("Closed binary file: %s", currentBinaryFileName.c_str().AsChar());
    }
}

void RealIPPP::CloseTextFile()
{
    if (textFileOpened && textFile.is_open())
    {
        textFile.close();
        textFileOpened = false;
        LOG_WRITE("Closed text file: %s", currentTextFileName.c_str().AsChar());
    }
}

void RealIPPP::WriteDataToTextFile(uint16 data[5])
{
    if (!textFileOpened || !textFile.is_open())
    {
        return;
    }

    textFile << std::setw(7) << std::right << wxString::Format("%u", data[0]) <<
        std::setw(7) << std::right << wxString::Format("%u", data[1]) <<
        std::setw(7) << std::right << wxString::Format("%u", data[2]) <<
        std::setw(7) << std::right << wxString::Format("%u", data[3]) <<
        std::setw(7) << std::right << wxString::Format("%u", (uint8)data[4]) << "\n";
}
