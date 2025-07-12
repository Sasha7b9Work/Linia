// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Application.h"
#include "Utils/Configurator.h"
#include "Communicator/ComPort/ComPort.h"
#include "Settings/Settings.h"
#include "Utils/Timer.h"
#include "MainWindow.h"
#include "Windows/ConsoleRS232.h"


wxIMPLEMENT_APP(Application);


wxString Application::file_name_config;


Application *Application::self = nullptr;


bool Application::OnInit()
{
    if (!wxApp::OnInit())
    {
        return false;
    }

    file_name_config = wxGetCwd() + "/CIC.conf";

    Log::Init();

    self = this;

    g_file_config = new wxFileConfig("", "", file_name_config);

    Config::SetFile("");

    SET::GUI::Load();

    // we use a PNG image in our HTML page
    wxImage::AddHandler(new wxPNGHandler);

    ConsoleRS232::Create();

    // create and show the main application window
    MainWindow *frame = new MainWindow(_L("ИППП 4"));

    frame->Show();

    Bind(wxEVT_TIMER, &Application::OnTimer, this, ID_TIMER);

    timer.SetOwner(this, ID_TIMER);

    timer.Start(10);

    return true;
}


void Application::Disable()
{
    LOG_WRITE("Application::Disable()");

    timer.Stop();

    SET::GUI::Save();
}


void Application::OnTimer(wxTimerEvent &)
{
    static std::mutex mutex;

    if (mutex.try_lock())
    {
        ComPort::Update();

        mutex.unlock();
    };
}


int Application::OnExit()
{
    LOG_WRITE("Application::OnExit()");

    wxConfigBase::Get(false)->Flush();

    wxConfigBase::Set(nullptr);

    SAFE_DELETE(g_file_config);

    return wxApp::OnExit();
}
