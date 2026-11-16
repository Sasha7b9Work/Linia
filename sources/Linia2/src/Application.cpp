// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Application.h"
#include "Settings/Settings.h"
#include "MainWindow.h"
#include "Communicator/UART/UART.h"
#include "Communicator/SPI/SPI.h"
#include "GUI/Dialogs/AutoRebootDialog.h"
#include "SoftTests/SoftTests.h"
#include "IPPP/Real/Chips.h"
#include "Windows/ConsoleRS232.h"
#include "Settings/FileJSON.h"
#include "Communicator/ClientHTTP/ClientHTTP.h"
#include "IPPP/IDevice.h"
#include "GUI/PageDebug/PanelRight.h"
#include "GUI/PageDebug/PageDebug.h"
#pragma warning(push, 0)
    #include <wx/msgdlg.h>
#pragma warning(pop)
#include <cstdlib>
#include <locale>
#ifndef _WIN32
    #include <csignal>
#endif


wxIMPLEMENT_APP(Application);


Application *Application::self = nullptr;


#ifndef _WIN32

static volatile sig_atomic_t g_signal_received = 0;

void Application::SignalHandler(int sig)
{
    // Устанавливаем флаг, а не делаем что-то сложное
    g_signal_received = sig;
}

static void CloseApplication()
{
    LOG_WRITE("CloseApplication()");

    if (Application::self)
    {
        if (MainWindow::self)
        {
            MainWindow::self->Close(true);
        }

        Application::self->Yield();

        Application::self->ExitMainLoop();
    }
}

#endif


#ifndef _WIN32
// Функция подавления определённых сообщений
static GLogWriterOutput MyLogWriter(GLogLevelFlags log_level,
    const GLogField *fields,
    gsize n_fields,
    gpointer user_data)
{
    const gchar *message = nullptr;
    const gchar *domain = nullptr;

    // Извлекаем поля из структуры
    for (gsize i = 0; i < n_fields; ++i)
    {
        if (g_strcmp0(fields[i].key, "MESSAGE") == 0)
            message = (const gchar *)fields[i].value;
        else if (g_strcmp0(fields[i].key, "GLIB_DOMAIN") == 0)
            domain = (const gchar *)fields[i].value;
    }

    if (domain && g_strcmp0(domain, "Gtk") == 0 &&
        message &&
        strstr(message, "gtk_box_gadget_distribute:") != nullptr &&
        strstr(message, "GtkCheckButton") != nullptr)
    {
        return G_LOG_WRITER_HANDLED;        // Подавляются сообщения, где содержатся gtk_box_gadget_distribute: и GtkCheckButton
    }

    // Все остальные сообщения передаем стандартному обработчику
    return g_log_writer_default(log_level, fields, n_fields, user_data);
}
#endif


bool Application::OnInit()
{
    Application::self = this;

#ifndef __WXMSW__
    g_log_set_writer_func(MyLogWriter, NULL, NULL);
#endif

    try
    {
        std::locale::global(std::locale(""));  // Установка системной локали
    }
    catch (const std::exception &)
    {
        // fallback: остаёмся на классической локали
    }

    setlocale(LC_ALL, "");

    if (!wxApp::OnInit())
    {
        return false;
    }

#ifndef __WXMSW__
    // Включаем отображение иконок в контекстных меню GTK3
    if (GtkSettings *settings = gtk_settings_get_default())
    {
        g_object_set(settings, "gtk-menu-images", TRUE, NULL);
    }
#endif

    SET::Init();

    Log::Init();

    Config::Init();

    SET::Load();

    // we use a PNG image in our HTML page
    wxImage::AddHandler(new wxPNGHandler);

    ConsoleRS232::Create();

    Chip::Init();

    IDevice::impl = IDevice::Create();

    if (!IDevice::impl)
    {
        wxMessageBox(L("Не удалось создать устройство"), L("Ошибка"), wxOK | wxICON_ERROR);
        Log::DeInit();
        Config::DeInit();
        return false;
    }

    // create and show the main application window
    new MainWindow(L("ИППП 4"));

    enum {
        ID_APP_TIMER = wxID_HIGHEST + 1
    };
    timer.SetOwner(this, ID_APP_TIMER);

    if (!UART::IsReady())
    {
        wxString message = wxString::Format(L("Устройство UART %s не обнаружено."), wxString(UART_DEVICE).c_str());

        LOG_ERROR("%s", message.ToUTF8().data());

        AutoRebootDialog dialog(MainWindow::self, message, 10, []
            {
                IGNORE_RESULT(std::system("shutdown -r now"));
            });

        dialog.ShowModal();
    }

    if (!SPI::IsReady())
    {
        wxString message = wxString::Format(L("Устройство SPI %s не обнаружено."), wxString(SPI_DEVICE).c_str());

        LOG_ERROR("%s", message.ToUTF8().data());

        AutoRebootDialog dialog(MainWindow::self, message, 10, []
            {
                IGNORE_RESULT(std::system("shutdown -r now"));
            });

        dialog.ShowModal();
    }

    timer.Start(10);

    if (!SoftTests::RunAll())
    {
        wxMessageBox(wxString::Format(L("Во время выполнения тестов произошли ошибки.\nДополнительная информация в файле %s."),
            Log::FileName().wc_str()), L("Ошибка"), wxOK | wxCENTRE | wxICON_ERROR);
    }

#ifdef _WIN32

//    ComPort::Connect(PanelUpper::self->GetNumPort());

#endif

    if (SET_DEBUG_MODE->GetBool())
    {
        MainWindow::self->SetMode(ModeMainWindow::Debug);
    }

    if (!GF::IsBoardOPi5Plus())
    {
//        MainWindow::self->Maximize(true);
    }

    MainWindow::self->Show();

    Bind(wxEVT_TIMER, [](wxTimerEvent &)
        {
            static std::mutex mutex;

            std::unique_lock<std::mutex> lock(mutex, std::try_to_lock);

            if (!lock.owns_lock())
            {
                return;
            }

#ifndef _WIN32
            int sig = g_signal_received;
            if (sig != 0)
            {
                g_signal_received = 0;
                LOG_WRITE("Received signal %d, closing", sig);
                CloseApplication();   // безопасно: в главном потоке
                return;
            }
#endif

            IDevice::impl->ApplicationTask();

            Log::PeriodicTask();

            if (PageDebug::self)
            {
                PageDebug::self->PeriodicTask();
            }

        }, ID_APP_TIMER);


    loggerHTTP.Connect();

#ifdef _WIN32
#else
    struct sigaction sa = {};
    sa.sa_handler = &Application::SignalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;  // не SA_RESTART — чтобы прервать блокирующие вызовы

    sigaction(SIGTERM, &sa, nullptr);
    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGHUP, &sa, nullptr);

//    signal(SIGTERM, [](int)
//        {
//            CloseApplication();
//        });
//
//    signal(SIGINT, [](int)
//        {
//            CloseApplication();
//        });
//
//    signal(SIGHUP, [](int)
//        {
//            CloseApplication();
//        });
#endif

    IDevice::impl->Init();

    IDevice::impl->SendCommand(":INFO");

    return true;
}


int Application::OnExit()
{
    timer.Stop();

    if (IDevice::impl)
    {
        IDevice::impl->DeInit();
        IDevice::impl = nullptr;
    }

    if (MainWindow::self)
    {
        MainWindow::self->Close(true);
    }

    LOG_WRITE("Exit");

    Config::DeInit();

    loggerHTTP.Disconnect();

    Log::DeInit();

    return wxApp::OnExit();
}
