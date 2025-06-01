// 2023/08/08 15:51:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define _L(x)     LL::ID(wxT(x))
#define _L_SAVE   LL::ID(wxT("Сохранить"))
#define _L_CLOSE  LL::ID(wxT("Закрыть"))


namespace LL
{
    wxString ID(const wxString &id);
}
