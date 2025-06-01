// 2024/08/12 14:09:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Reader/PasswordSender.h"
#include "Reader/Reader.h"
#include "DataBase/DataBasePasswords.h"


namespace PasswordSender
{
    namespace BrutForce                         // Перебор паролей
    {
        static const int NUMBERS_TRANS = 2;     // Столько раз передаём каждый пароль
        static int number_password = 0;         // Номер передаваемого пароля
        static uint64 last_sended_password = 0; // Последний переданный пароль

        static void Reset()
        {
            number_password = 0;
        }

        static bool IsFinished()
        {
            return ((number_password / NUMBERS_TRANS) == PasswDB::Count());
        }

        static uint64 NextPassword()
        {
            last_sended_password = PasswDB::GetPassword(number_password++ / NUMBERS_TRANS);

            return last_sended_password;
        }

        static uint64 LastSendedPassword()
        {
            return last_sended_password;
        }

        static int ProcessedPasswords()
        {
            return (number_password - 1) / NUMBERS_TRANS + 1;
        }
    };
}


void PasswordSender::ResetAndStart()
{
    BrutForce::Reset();

    Reader::Send("#AUTH DISABLE");

    Card::Eject();
}


void PasswordSender::EndProcess()
{
    BrutForce::Reset();
}


bool PasswordSender::IsFinished()
{
    return BrutForce::IsFinished();
}


void PasswordSender::AuthNext()
{
    Reader::SendFormat("#AUTH %llu", BrutForce::NextPassword());
}


int PasswordSender::ProcessedPasswords()
{
    return BrutForce::ProcessedPasswords();
}

wxString PasswordSender::CurrentPassword()
{
    return wxString::Format("%llu", BrutForce::LastSendedPassword());
}
