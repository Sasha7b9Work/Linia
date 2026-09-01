// 2026/09/01 16:36:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "IPPP/IDevice.h"
#include "Communicator/UART/UART.h"
#include "Communicator/GPIO/GPIO.h"
#include "Communicator/SPI/SPI.h"
#include "IPPP/Real/Chips.h"
#include "IPPP/SCPI/SCPI.h"
#include "Communicator/ComPort/ComPort.h"
#include "IPPP/Real/RealDevice.h"
#include "IPPP/Keyboard/Keyboard.h"
#include "GUI/PageDebug/PageDebug.h"
#include "Application.h"
#include "GUI/PageDebug/PanelRight.h"
#include "Utils/Timer.h"
#include <cstdarg>
#include <cstring>


IDevice *IDevice::impl = nullptr;


bool need_write_data_to_file = false;


BackgroundWorker RealDevice::worker{ BackgroundTask, 1 };


RealDevice::~RealDevice()
{
    DeInit();
}


IDevice *IDevice::Create()
{
    return new RealDevice();
}


bool RealDevice::Init()
{
    GPIO::Init();

    SPI::Init();

    bool result = false;

    if (UART::Init(SCPI::OnEventCallback))
    {
        connected = true;
        running = true;
        result = true;
    }

    Keyboard::Init(FuncOnKeyStart, FuncOnKeyStop, FuncOnEncoder);

    worker.Start();

    return result;
}


void RealDevice::ApplicationTask()
{
    SCPI::ApplicationTask();
}


void RealDevice::DeInit()
{
    worker.Stop();

    Keyboard::DeInit();

    running = false;

    UART::DeInit();

    connected = false;
}


bool RealDevice::IsConnected() const
{
    return connected;
}


void RealDevice::SendCommand(pchar format, ...) const
{
    char message[1024];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    std::strcat(message, "\0");

#ifdef WIN32
    ComPort::Send(message, (int)std::strlen(message) + 1);
#else
    UART::SendBuffer(message, (int)std::strlen(message) + 1);
#endif
}


void RealDevice::FuncOnKeyStart(bool state)
{
    state = !state;

    TheApp->CallAfter([state]()
        {
            ThePageDebug->labelButtonStart->SetLabel(state ? "ИЗМЕРЕНИЕ \"ВКЛ\"" : "ИЗМЕРЕНИЕ \"ОТКЛ\"");
        });
}


void RealDevice::FuncOnKeyStop(bool state)
{
    state = !state;

    TheApp->CallAfter([state]()
        {
            ThePageDebug->labelButtonStop->SetLabel(state ? "СТОП \"ВКЛ\"" : "СТОП \"ОТКЛ\"");
        });
}


void RealDevice::FuncOnEncoder(int delta)
{
    TheApp->CallAfter([delta]()
        {
            int value = 0;
            ThePageDebug->labelEncoder->GetLabel().ToInt(&value);

            ThePageDebug->labelEncoder->SetLabel(wxString::Format("%d", value + delta));
        });
}


void RealDevice::BackgroundTask()
{
    static int data_dac[NUMBER_ADC][POINTS_IN_SAMPLE_ADC];
    static int data_code[POINTS_IN_SAMPLE_ADC];

    TheApp->CallAfter([]()
        {
            if (IDevice::impl->ReadData(data_dac, data_code))
            {
                LOG_WRITE("read data");

                for (int i = 0; i < NUMBER_ADC; i++)
                {
                    ThePanelRight->data[i]->SetData(data_dac[i]);
                }

                ThePanelRight->data[NUMBER_ADC]->SetData(data_code);
            }
        });
}


bool RealDevice::ReadData(int data_dac[NUMBER_ADC][POINTS_IN_SAMPLE_ADC], int data_code[POINTS_IN_SAMPLE_ADC])
{
    static int64 prev_time = Timer::CurrentTimeMS();

    if (Timer::CurrentTimeMS() - prev_time < 1000)
    {
        return false;
    }

    prev_time = Timer::CurrentTimeMS();

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

    if (prev == false)
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

    prev = false;

    return result;
}


void RealDevice::OpenNewBinaryFile()
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

void RealDevice::OpenNewTextFile()
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

void RealDevice::CloseBinaryFile()
{
    if (binaryFileOpened && binaryFile.is_open())
    {
        binaryFile.flush();
        binaryFile.close();
        binaryFileOpened = false;
        LOG_WRITE("Closed binary file: %s", currentBinaryFileName.c_str().AsChar());
    }
}

void RealDevice::CloseTextFile()
{
    if (textFileOpened && textFile.is_open())
    {
        textFile.flush();
        textFile.close();
        textFileOpened = false;
        LOG_WRITE("Closed text file: %s", currentTextFileName.c_str().AsChar());
    }
}

void RealDevice::WriteDataToTextFile(int16 data[5])
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
