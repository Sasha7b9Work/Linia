// 2023/08/31 11:42:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Reader/Reader.h"
#include "Communicator/ComPort.h"
#include "Utils/String.h"
#include "DataBase/DataBasePasswords.h"
#include "Panels/ConsoleRS232.h"
#include "Utils/StringUtils.h"
#include "Panels/CommonPanel.h"
#include "Utils/Timer.h"
#include "Utils/GlobalFunctions.h"
#include "Reader/Card.h"
#include "Settings/Settings.h"
#include "Utils/Configurator.h"
#include "Utils/EventsLog.h"
#include "DataBase/DataBaseCards.h"
#include "Utils/String.h"
#include "Reader/Indicator.h"
#include "Reader/PasswordSender.h"
#include "Panels/Notebook/PageUser/PageUser.h"
#include "Panels/Notebook/PageMaster/PageMaster.h"
#include "Panels/Dialogs/KeysSL0Dialog.h"
#include "Panels/Notebook/PageMaster/ColorDialog.h"
#include <cstdlib>
#include <vector>
#include <ctime>


namespace Reader
{
    struct Connector
    {
        static void _CallbackOnConnect()
        {
            is_connected = true;
            CommonPanel::self->CallbackOnReaderConnected(is_connected);
            time_last_recv = wxGetLocalTime();
        }

        static void CallbackPeriodic(pchar answer)
        {
            (void)answer;
            /*
            if (is_connected)
            {
                if (answer[0])
                {
                    time_last_recv = wxGetLocalTime();
                    ping_sended = false;
                }
                else
                {
                    if (wxGetLocalTime() - time_last_recv > 2)
                    {
                        time_disconnect = (uint)std::clock() / CLK_TCK;
                        is_connected = false;
                        ComPort::Close();
                        CommonPanel::self->CallbackOnReaderConnected(is_connected);
                    }
                    else if (wxGetLocalTime() - time_last_recv > 1)
                    {
                        Reader::Send("#MODE?");
                        ping_sended = true;
                    }
                }
            }
            */
        }

        static bool IsConnected()
        {
            return is_connected;
        }

        static void Disconnect()
        {
            is_connected = false;
            time_disconnect = Timer::CurrentTimeSec();
        }

        static uint time_disconnect;

    private:
        static bool is_connected;
        static long time_last_recv;
        static bool ping_sended;
    };

    bool Connector::is_connected = false;
    long Connector::time_last_recv = 0;
    bool Connector::ping_sended = false;
    uint Connector::time_disconnect = Timer::CurrentTimeSec();

    uint TimeFromDisconnect()
    {
        return Timer::CurrentTimeSec() - Connector::time_disconnect;
    }

    static ReaderInfo info_reader;

    static void ProcessInfo(pchar answer);          // INFO
    static void ProcessConfig(pchar answer);        // CONFIG
    static void ProcessCard(pchar answer);          // CARD
    static void ProcessCardFail(const Words &);
    static void ProcessMake(const Words &);         // MAKE
    static void ProcessKEYSMFSL0(const Words &);    // KEYSMFSL0

    static bool ends_with(const std::string &str, const std::string &suffix)
    {
        // Проверим, если длина суффикса больше строки
        if (suffix.length() > str.length())
        {
            return false;
        }
        // Сравним конец основной строки с суффиксом
        return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
    }
}


void Reader::Send(pchar text)
{
    static std::mutex mutex;

    mutex.lock();

    static clock_t prev_time = std::clock();

    Log::ToDevice((char *)text);

    ConsoleRS232::self->AddLine(wxString::Format("%s%s", DIRECT_PROMT, text));

    String message("%s\x0a", text);

    while (std::clock() < prev_time + 100)
    {
    }

    prev_time = std::clock();

    ComPort::Send(message.c_str());

    mutex.unlock();
}


void Reader::SendFormat(const char *format, ...)
{
    char message[2048];
    std::va_list args;
    va_start(args, format);
    vsprintf(message, format, args);
    va_end(args);

    Send(message);
}


void Reader::SendBuffer(void *buffer, int num_bytes)
{
    static int all_bytes = 0;
    all_bytes += num_bytes;

    ComPort::Send(buffer, num_bytes);
}


void Reader::UpdateOnReceive(pchar answer)
{
    Connector::CallbackPeriodic(answer);

    if (answer[0] == '\0')
    {
        return;
    }

    Words words(answer, " \n=");

    if (!Connector::IsConnected() && words.Size() == 1 && words.At(0) == "#OK")
    {
        Reader::Send("#INFO");
    }
    else if (words.Consist("INFO"))
    {
        ProcessInfo(answer);
    }
    else if (words.Consist("VER"))
    {
        ProcessConfig(answer);
    }
    else if (words.At(0) == "#CARD")
    {
        ProcessCard(answer);
    }
    else if (words.At(0) == "#MAKE")
    {
        ProcessMake(words);
    }
    else if (words.At(0) == "#KEYSMFSL0")
    {
        ProcessKEYSMFSL0(words);
    }
}


void Reader::ProcessInfo(pchar answer)
{
    Parameters parameters;

    SU::SplitToParameters(answer, parameters);

    if (parameters.parameters.size() > 6)
    {
        info_reader.product_type = parameters.Find("HW");
        info_reader.serial_number = parameters.Find("SN");
        info_reader.version_hardware = parameters.Find("VH");
        info_reader.version_loader = parameters.Find("VB");
        info_reader.version_software = parameters.Find("VS") + " (" + parameters.Find("BUILD") + ")";
        info_reader.data_manufacture = parameters.Find("DT");
        info_reader.eco_enabled = (parameters.FindInt("ECO EN") > 0);
        info_reader.osdp_address = parameters.Find("OSDP_ADDR");
        info_reader.osdp_baudrate = parameters.Find("OSDP_BAUD");
        info_reader.info = parameters.Find("INFO");
        info_reader.time_build = parameters.Find("TIME BUILD");

        Connector::_CallbackOnConnect();

        PasswordSender::ResetAndStart();                        // Начинаем процесс перебора паролей

        CommonPanel::self->CallbackOnReaderConnected(true);
    }
}


void Reader::ProcessConfig(pchar answer)
{
    Parameters parameters;

    SU::SplitToParameters(answer, parameters);

    wxString red = parameters.Find("RED");
    wxString green = parameters.Find("GREEN");

    ColorDialog::StoreReaderColors(red, green);
}


void Reader::ProcessCard(pchar answer)
{
    Indicator::Task();

    Words words(answer, " ");

    Parameters parameters;

    SU::SplitToParameters(answer, parameters, " \n");

    if (words.Size() > 1)
    {
        static std::string detected_UID;                // / Здесь сохраняем тип карты, которая поднесена
        static std::string detected_type;               // / Про то, что на ней записано, ещё ничего не известно

        if (words.At(1) == "DETECTED")
        {
            detected_UID = parameters.Find("UID");
            detected_type = parameters.Find("TYPE");
        }
        else if (words.At(1) == "REMOVE")
        {
            Card::Eject();
            PasswordSender::ResetAndStart();
        }
        else if(words.At(1) == "READ")
        {
            if (ends_with(detected_type, "SL0") || ends_with(detected_type, "SL1"))
            {
                PasswordSender::EndProcess();
                Card::InsertUnknown(detected_UID, detected_type);
            }
            else
            {
                if (words.Size() > 5 && words.At(5) == "OK")                                    // CARD READ OK
                {
                    if (words.Consist("MASTER"))
                    {
                        if (!Card::InsertMaster(words.At(2), parameters.Find("TYPE"), words.At(7)))
                        {
                            Card::InsertUnknown(detected_UID, detected_type);
                        }
                    }
                    else
                    {
                        Card::InsertUser(words.At(2), parameters.Find("TYPE"), parameters.Find("NUMBER"));
                    }

                    PasswordSender::EndProcess();
                }
                else                                                                            // CARD READ FAIL
                {
//                    LOG_WRITE(" ");

                    ProcessCardFail(words);

//                    LOG_WRITE(" ");

                    if (PasswordSender::IsFinished() && !Card::IsMaster() && !Card::IsUser())
                    {
//                        LOG_WRITE(" ");

                        Card::InsertUnknown(detected_UID, detected_type);
                    }
                }
            }
        }
    }
}


void Reader::ProcessCardFail(const Words &words)
{
    if (words.At(words.Size() - 2) == "DISABLED")                                        // Подбор пароля начинаем всегда с отключения авторизации
    {                                                               // Если авторизация была отключена - добавляем в базу паролей текущий пароль карты
        PasswDB::Append(gset.password_card.value);
    }

    if (PasswordSender::IsFinished())
    {
        Indicator::Card();
    }
    else
    {
        PasswordSender::AuthNext();

//        LOG_WRITE("point 2");

        GF::FindAnyButton(CommonPanel::self, ID_BUTTON_CARD_INFO)->
            SetLabel(String("%s %d/%d", wxString(_L("Идёт подключение")).c_str().AsChar(), PasswordSender::ProcessedPasswords(), PasswDB::Count()).c_str());
    }
}


void Reader::ProcessMake(const Words &words)
{
    do
    {
        Indicator::Task();

        if (words.At(1) == "USER")
        {
            if (words.Last() == "FAIL")
            {
                GF::FindAnyButton(PageUser::self, ID_BUTTON_WRITE_USER_CARD)->SetLabel(_L("Ошибка записи"));
                GF::FindAnyButton(PageUser::self, ID_BUTTON_WRITE_USER_CARD)->Enable(false);

//            wxMessageDialog dlg(PageUser::self, _L("При записи пользовательской карты произошла ошибка."), _L("Ошибка записи"), wxCENTRE | wxICON_ERROR | wxOK);
//            dlg.ShowModal();
            }
            else
            {
                CardInfo info;

                if (words.Size() == 7)
                {
                    info.number = SU::UInt64FromString(words.At(5).c_str());

                    PageUser::self->SetNextCardNumber();
                }
                else
                {
                    if (!CardsDB::ReadCard(Card::GetUID().GetFactory(), &info))
                    {
                        break;
                    }
                }

                info.id = Card::GetUID().GetFactory();

                info.time = CardInfo::TimeNow();

                CardsDB::AppendCard(info);

                EventsLog::Append(info);

                PasswDB::Append(SU::UInt64FromString(words.At(3).c_str()));
            }
        }
        else if (words.At(1) == "MASTER")
        {
            if (words.Last() == "FAIL")
            {
                GF::FindAnyButton(PageMaster::self, ID_BUTTON_WRITE_MASTER_CARD)->SetLabel(_L("Ошибка записи"));
                GF::FindAnyButton(PageMaster::self, ID_BUTTON_WRITE_MASTER_CARD)->Enable(false);
//            wxMessageBox(wxT("При записи мастер-карты произошла ошибка."), wxT("Ошибка записи"), wxICON_ERROR);
            }
            else
            {
                gset.password_master.value = gset.password_current.value;

                Config::WriteUInt64(gset.password_master.key, gset.password_master.value);
            }
        }
    } while (false);
}


void Reader::ProcessKEYSMFSL0(const Words &words)
{
    if (words.At(1) == "WRITE")
    {
        KeysSL0Dialog::self->ShowInfoDialog(words.At(2) == "OK");
    }

    KeysSL0Dialog::self->SetStateAllButtons(true);
}


bool Reader::IsConnected()
{
    return Connector::IsConnected();
}


ReaderInfo Reader::GetInfo()
{
    return info_reader;
}


void Reader::Disconnect()
{
    ComPort::Close();
    Connector::Disconnect();
}
