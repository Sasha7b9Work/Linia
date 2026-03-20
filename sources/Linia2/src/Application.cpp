// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Application.h"
#include "Utils/Configurator.h"
#include "Settings/Settings.h"
#include "MainWindow.h"
#include "Windows/ConsoleRS232.h"
#include "IPPP/Tests/Tests.h"
#include "SoftTests/SoftTests.h"
#include "Communicator/UART/UART.h"
#include "Communicator/SPI/SPI.h"
#include "Controls/AutoRebootDialog.h"
#include "IPPP/Real/Chips.h"
#include "Panels/PanelUpper.h"
#include "Communicator/ComPort/ComPort.h"
#include "IPPP/I_IPPP.h"
#include "IPPP/Real/RealIPPP.h"
#include <cstdlib>


wxIMPLEMENT_APP(Application);


Application *TheApp = nullptr;


class NullLog : public wxLog
{
public:
    virtual void DoLogRecord(wxLogLevel,
        const wxString &,
        const wxLogRecordInfo &) override
    {
        // Ничего не делаем - просто игнорируем все логи
    }
};


bool Application::OnInit()
{
    TheApp = this;

    // Попытка отключить предупреждения вида "Gtk-WARNING"
    wxLog::SetActiveTarget(new NullLog());

    // Попытка отключить предупреждения вида "Gtk-WARNING"
    // Устанавливаем переменные окружения для GTK (для Linux)
#ifndef __WXMSW__
    setenv("G_MESSAGES_DEBUG", "0", 1);
    setenv("GTK_DEBUG", "0", 1);
    setenv("NO_AT_BRIDGE", "1", 1);
#endif

    std::locale::global(std::locale(""));  // Установка системной локали
    setlocale(LC_ALL, "");

    if (!wxApp::OnInit())
    {
        return false;
    }

#ifndef __WXMSW__
    // Включаем отображение иконок в контекстных меню GTK3
    GtkSettings *settings = gtk_settings_get_default();
    g_object_set(settings, "gtk-menu-images", TRUE, NULL);
#endif

    Log::Init();

    Config::Init();

    SET::Load();

    // we use a PNG image in our HTML page
    wxImage::AddHandler(new wxPNGHandler);

    ConsoleRS232::Create();

    Chip::Init();

    // create and show the main application window
    MainWindow *frame = new MainWindow(wxT("ИППП 4"));

    Bind(wxEVT_TIMER, &Application::OnTimer, this, timer.GetId());

    timer.SetOwner(this, timer.GetId());

    Test::Load("example.tst");

    frame->Show();

    if (!UART::IsAvailability())
    {
        wxString message = wxString::Format(wxT("Устройство UART %s не обнаружено."), UART_DEVICE);

        LOG_ERROR(message.c_str().AsChar());

        AutoRebootDialog dialog(frame, message, 10, []
            {
                IGNORE_RESULT(std::system("shutdown -r now"));
            });

        dialog.ShowModal();
    }

    if (!SPI::IsAvailability())
    {
        wxString message = wxString::Format(wxT("Устройство SPI %s не обнаружено."), SPI_DEVICE);

        LOG_ERROR(message.c_str().AsChar());

        AutoRebootDialog dialog(frame, message, 10, []
            {
                IGNORE_RESULT(std::system("shutdown -r now"));
            });

        dialog.ShowModal();
    }

    I_IPPP::Create();

    timer.Start(10);

    if (!SoftTests::RunAll())
    {
        wxMessageBox(wxString::Format(_("Во время выполнения тестов произошли ошибки.\n") +
            _("Дополнительная информация в файле %s."), Log::FileName().c_str().AsChar()), wxT("Ошибка"), wxOK | wxCENTRE | wxICON_ERROR);
    }

#ifdef WIN32

    ComPort::Connect(PanelUpper::self->GetNumPort());

#endif

    return true;
}


void Application::Disable()
{
    timer.Stop();

    SET::Save();
}


void Application::OnTimer(wxTimerEvent &)
{
    static std::mutex mutex;

    if (mutex.try_lock())
    {
        I_IPPP::impl->Update();

        TheMainWindow->Update();

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
