// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Application.h"
#include "Utils/Configurator.h"
#include "Communicator/ComPort/ComPort.h"
#include "Settings/Settings.h"
#include "Utils/Timer.h"
#include "MainWindow.h"
#include "Windows/ConsoleRS232.h"
#include "IPPP/Tests/Tests.h"
#include "Communicator/GPIO/GPIO.h"
#include "Display/PanelErrors.h"
#include "IPPP/RealIPPP.h"
#include "IPPP/EmulatorIPPP.h"
#include "SoftTests/SoftTests.h"
#include "Communicator/UART/UART.h"
#include "Communicator/SPI/SPI.h"
#include "Controls/AutoRebootDialog.h"
#include "IPPP/Device/Chips.h"
#include <cstdlib>


wxIMPLEMENT_APP(Application);


Application *Application::self = nullptr;


class FilteredStreambuf : public std::streambuf {
    std::streambuf *original;

public:
    FilteredStreambuf(std::streambuf *orig) : original(orig)
    {
    }

protected:
    virtual int overflow(int c) override
    {
        // Можно добавить фильтрацию по содержимому
        return original->sputc(c);
    }

    virtual std::streamsize xsputn(const char *s, std::streamsize n) override
    {
        // Фильтруем ненужные сообщения GTK
        if (std::strstr(s, "Gtk") || std::strstr(s, "gtk") ||
            std::strstr(s, "GDK") || std::strstr(s, "gdk"))
        {
            return n; // Игнорируем
        }
        return original->sputn(s, n);
    }
};


bool Application::OnInit()
{
    std::locale::global(std::locale(""));  // Установка системной локали
    setlocale(LC_ALL, "");

    FilteredStreambuf filtered_stderr(std::cerr.rdbuf());
    std::cerr.rdbuf(&filtered_stderr);

#ifndef WIN32

    // Устанавливаем переменные окружения для GTK
    ::setenv("G_MESSAGES_DEBUG", "0", 1);
    ::setenv("GTK_DEBUG", "0", 1);
    ::setenv("GDK_DEBUG", "0", 1);
    ::setenv("NO_AT_BRIDGE", "1", 1);

    // Отключаем логирование в wxWidgets
//    wxLog::SetActiveTarget(new wxLogNull);

    // Дополнительно: отключаем X11 warnings
    ::setenv("XLIB_SKIP_ARGB_VISUALS", "1", 1);

#endif

    if (!wxApp::OnInit())
    {
        return false;
    }

    Log::Init();

    self = this;

    Config::Init();

    SET::GUI::Load();

    // we use a PNG image in our HTML page
    wxImage::AddHandler(new wxPNGHandler);

    ConsoleRS232::Create();

    Chip::Init();

    // create and show the main application window
    MainWindow *frame = new MainWindow("ИППП 4");

    Bind(wxEVT_TIMER, &Application::OnTimer, this, timer.GetId());

    timer.SetOwner(this, timer.GetId());

    Test::Load("example.tst");

    frame->Show();

    if (!UART::IsAvailability())
    {
        AutoRebootDialog dialog(frame, wxString::Format("Устройство UART %s не обнаружено.", UART_DEVICE), 10, []
            {
                (void)std::system("reboot -f");
            });

        dialog.ShowModal();
    }

    if (!SPI::IsAvailability())
    {
        AutoRebootDialog dialog(frame, wxString::Format("Устройство SPI %s не обнаружено.", SPI_DEVICE), 10, []
            {
                (void)std::system("reboot -f");
            });

        dialog.ShowModal();
    }

    I_IPPP::impl = new RealIPPP();
    I_IPPP::impl->Init();

    timer.Start(10);

    PanelErrors::self->AppendError(Error::_1, "Мало памяти");
    PanelErrors::self->AppendError(Error::_2, "Много памяти");

    for (int i = 0; i < 10; i++)
    {
        PanelErrors::self->AppendError(Error::_1, wxString::Format("Ошибка %d", i));
    }

//    PanelErrors::self->RemoveError(Error::_1, "Мало памяти");
//    PanelErrors::self->RemoveError(Error::_2, "Много памяти");

    if (!SoftTests::RunAll())
    {
        wxMessageBox(wxString::Format("Во время выполнения тестов произошли ошибки.\n"
            "Дополнительная информация в файле %s.", Log::FileName().c_str().AsChar()), "Ошибка", wxOK | wxCENTRE | wxICON_ERROR);
    }

    LOG_WRITE_TRACE(" ");

    return true;
}


void Application::Disable()
{
    timer.Stop();

    SET::GUI::Save();
}


void Application::OnTimer(wxTimerEvent &)
{
    static std::mutex mutex;

    if (mutex.try_lock())
    {
        ComPort::Update();

        I_IPPP::impl->Update();

        MainWindow::self->Update();

        mutex.unlock();
    };
}


int Application::OnExit()
{
    Log::DeInit();

    Config::DeInit();

    return wxApp::OnExit();
}


void Application::OnReceiveUART(uint8)
{

}


void Application::OnGovernor(int)
{

}


void Application::OnButtonStart(bool /*press*/)
{

}


void Application::OnButtonStop(bool /*press*/)
{

}


void Application::OnReadData(std::vector<int>(&)[4])
{

}
