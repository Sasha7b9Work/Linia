// 2024/08/12 14:09:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
* Управляет засылаемыми паролями
*/


namespace PasswordSender
{
    // Сбросить список паролей и начать перебор
    void ResetAndStart();

    bool IsFinished();

    // Завершить процесс перебора паролей
    void EndProcess();

    void AuthNext();

    // Столько паролей уже обработано
    int ProcessedPasswords();

    // Последний применённый пароль
    wxString CurrentPassword();
}
