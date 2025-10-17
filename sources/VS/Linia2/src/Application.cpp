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


wxIMPLEMENT_APP(Application);


Application *Application::self = nullptr;


bool Application::OnInit()
{
    std::locale::global(std::locale(""));  // Установка системной локали
    setlocale(LC_ALL, "");

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

    // create and show the main application window
    MainWindow *frame = new MainWindow("ИППП 4");

    Bind(wxEVT_TIMER, &Application::OnTimer, this, timer.GetId());

    timer.SetOwner(this, timer.GetId());

    Test::Load("example.tst");

    frame->Show();

    LOG_WRITE_TRACE(" ");

    if (UART::IsAvailability())
    {
        int result = wxMessageBox(wxString::Format("Устройство UART не обнаружено. Перезагрузка через %d секунд", 5),
            "Ошибка", wxCANCEL | wxOK | wxCENTRE | wxICON_ERROR);

        if (result == wxOK)
        {
            (void)std::system("reboot -f");
        }
    }

    I_IPPP::impl = new RealIPPP();
    I_IPPP::impl->Init();

    timer.Start(10);

    PanelErrors::self->AppendError(Error::_1, "Мало памяти");
    PanelErrors::self->AppendError(Error::_2, "Много памяти");

    PanelErrors::self->RemoveError(Error::_1, "Мало памяти");
    PanelErrors::self->RemoveError(Error::_2, "Много памяти");

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
