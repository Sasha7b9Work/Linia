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
#include <cstdarg>
#include <cstring>


IDevice *IDevice::impl = nullptr;


RealDevice::~RealDevice()
{
    Shutdown();
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

    return result;
}


void RealDevice::ApplicationTask()
{
    SCPI::ApplicationTask();
}


void RealDevice::Shutdown()
{
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
            ThePageDebug->labelButtonStart->SetLabel(state ? "Start ВКЛ" : "Start ОТКЛ");
        });
}


void RealDevice::FuncOnKeyStop(bool state)
{
    state = !state;

    TheApp->CallAfter([state]()
        {
            ThePageDebug->labelButtonStop->SetLabel(state ? "Stop ВКЛ" : "Stop ОТКЛ");
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
