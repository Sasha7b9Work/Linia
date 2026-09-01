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
#include "Utils/BackgroundWorker.h"
#include "Application.h"
#include <iomanip>
#include <bit>


bool need_write_data_to_file = false;

namespace RealIPP_NS
{
    static void BackgroundTask();
    static BackgroundWorker worker{ BackgroundTask, 1 };
}


void RealIPPP::ApplicationTask()
{
    IDevice::impl->ApplicationTask();
}


void RealIPP_NS::BackgroundTask()
{
    static int data_dac[NUMBER_ADC][POINTS_IN_SAMPLE_ADC];
    static int data_code[POINTS_IN_SAMPLE_ADC];

    TheApp->CallAfter([]()
        {
            if (I_IPPP::GetInstance()->ReadData(data_dac, data_code))
            {
                for (int i = 0; i < NUMBER_ADC; i++)
                {
                    ThePanelRight->data[i]->SetData(data_dac[i]);
                }

                ThePanelRight->data[NUMBER_ADC]->SetData(data_code);
            }
        });
}


bool RealIPPP::ReadData(int data_dac[NUMBER_ADC][POINTS_IN_SAMPLE_ADC], int data_code[POINTS_IN_SAMPLE_ADC])
{
    auto OpenFilesForWrite = [this]()
        {
            if (need_write_data_to_file)
            {
                OpenNewBinaryFile();
                OpenNewTextFile();
            }
        };

    auto WriteToFiles = [this](int16 data[5])
        {
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
        };

    auto CloseFilesForWrite = [this]()
    {
        if (need_write_data_to_file)
        {
            void CloseBinaryFile();
            void CloseTextFile();

            need_write_data_to_file = false;
        }
    };




    bool result = false;

    static bool prev = false;

    if (pinFIFO_FULL.GetState() && prev == false)
    {
        OpenFilesForWrite();

        for (int i = 0; i < POINTS_IN_SAMPLE_ADC; i++)
        {
            uint16 data[5];

            SPI::ReadFPGA((uint8 *)data, NUMBER_ADC * 2 + 1);

            for (int num = 0; num < 4; num++)
            {
                uint16 d = data[num];

                data[num] = (uint16)((d >> 8) | (d << 8));
            }

            WriteToFiles((int16 *)data);

            for (int num_dac = 0; num_dac < 4; num_dac++)
            {
                int16 *pointer = (int16 *)&data[num_dac];

                data_dac[num_dac][i] = *pointer;
            }

            data_code[i] = (uint8)data[4];
        }

        CloseFilesForWrite();

        result = true;
    }

    prev = pinFIFO_FULL.GetState();

    return result;
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

#ifdef _WIN32
    wxString basePath = "D:\\";
#else
    wxString basePath = "/mnt/nvme/data/";
#endif

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
        binaryFile.flush();
        binaryFile.close();
        binaryFileOpened = false;
        LOG_WRITE("Closed binary file: %s", currentBinaryFileName.c_str().AsChar());
    }
}

void RealIPPP::CloseTextFile()
{
    if (textFileOpened && textFile.is_open())
    {
        textFile.flush();
        textFile.close();
        textFileOpened = false;
        LOG_WRITE("Closed text file: %s", currentTextFileName.c_str().AsChar());
    }
}

void RealIPPP::WriteDataToTextFile(int16 data[5])
{
    if (!textFileOpened || !textFile.is_open())
    {
        return;
    }

    textFile << std::setw(7) << std::right << wxString::Format("%d", data[0]) <<
        std::setw(7) << std::right << wxString::Format("%d", data[1]) <<
        std::setw(7) << std::right << wxString::Format("%d", data[2]) <<
        std::setw(7) << std::right << wxString::Format("%d", data[3]) <<
        std::setw(7) << std::right << wxString::Format("%u", (uint8)data[4]) << "\n";
}
