// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Application.h"
#include "Device.h"
#include "Utils/Configurator.h"
#include "Communicator/ComPort.h"
#include "Panels/ConsoleRS232.h"
#include "DataBase/DataBasePasswords.h"
#include "Settings/Settings.h"
#include "Utils/Timer.h"
#include "Panels/ListPasswords.h"
#include "DataBase/DataBaseCards.h"
#include "MainWindow.h"
#include "DataBase/Server/Server.h"


wxIMPLEMENT_APP(Application);


wxString Application::file_name_config;


Application *Application::self = nullptr;


bool Application::OnInit()
{
    if (!wxApp::OnInit())
    {
        return false;
    }

    SetWorkingDirectory();

    file_name_config = wxGetCwd() + "/CIC.conf";

    Log::Init();

    self = this;

    g_file_config = new wxFileConfig("", "", file_name_config);

    Config::SetFile("");

    SET::GUI::Load();

    // we use a PNG image in our HTML page
    wxImage::AddHandler(new wxPNGHandler);

    PasswDB::Init();

    gset.password_card.value = PasswDB::GetPassword(0);

    ConsoleRS232::Create();

    // create and show the main application window
    MainWindow *frame = new MainWindow("Card Issuing Center - CIC");

    frame->Show();

    Bind(wxEVT_TIMER, &Application::OnTimer, this, ID_TIMER);

    MainWindow::self->Preprocess();

    timer.SetOwner(this, ID_TIMER);

    timer.Start(10);

    Server::Create();

    CardsDB::LoadDataBase();

    return true;
}


void Application::Disable()
{
    LOG_WRITE("Application::Disable()");

    timer.Stop();
}


void Application::OnTimer(wxTimerEvent &)
{
    static std::mutex mutex;

    if (mutex.try_lock())
    {
        Server::Update();

        Device::Update();

        ComPort::Update();

        mutex.unlock();
    };
}


int Application::OnExit()
{
    LOG_WRITE("Application::OnExit()");

    PasswDB::DeInit();

    wxConfigBase::Get(false)->Flush();

    wxConfigBase::Set(nullptr);

    SAFE_DELETE(g_file_config);

    CardsDB::SaveDataBase();

    Server::Destroy();

    return wxApp::OnExit();
}


void Application::SetWorkingDirectory()
{
#ifdef WIN32
    // В Windows рабочий каталог - тот, в котором находится исполняемый файл.
#else
    wxSetWorkingDirectory("/var/lib/cic");
#endif
}
