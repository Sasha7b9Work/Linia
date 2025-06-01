// 2023/09/01 12:08:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Reader/Card.h"


namespace PasswDB
{
    void Init();

    void SetNameFile(const wxString &);

    void DeInit();

    void Append(uint64);

    int Count();

    uint64 GetPassword(int);
}
