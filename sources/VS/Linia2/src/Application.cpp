// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Application.h"
#include "Utils/Configurator.h"
#include "Communicator/ComPort/ComPort.h"
#include "Settings/Settings.h"
#include "Utils/Timer.h"
#include "MainWindow.h"
#include "Windows/ConsoleRS232.h"
#ifndef WIN32
//#include <gtk/gtk.h>
#endif


wxIMPLEMENT_APP(Application);


wxString Application::file_name_config;


Application *Application::self = nullptr;


#ifndef WIN32
void g_log_set_handler(const gchar *log_domain, GLogLevelFlags log_levels,
    GLogFunc log_func, gpointer user_data);

static void suppress_gtk_warnings(const gchar *log_domain,
    GLogLevelFlags log_levels,
    const gchar *message,
    gpointer user_data)
{
    if (g_strrstr(message, "smaller than min-size") != nullptr)
        return; // Игнорируем это конкретное предупреждение

    // Для других сообщений используем стандартный обработчик
    g_log_default_handler(log_domain, log_levels, message, user_data);
}
#endif


bool Application::OnInit()
{
#ifndef WIN32
    g_log_set_handler("Gtk", G_LOG_LEVEL_WARNING,
        suppress_gtk_warnings, NULL);
#endif

    std::locale::global(std::locale(""));  // Установка системной локали
    setlocale(LC_ALL, "");

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

    Bind(wxEVT_TIMER, &Application::OnTimer, this, timer.GetId());

    timer.SetOwner(this, timer.GetId());

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
