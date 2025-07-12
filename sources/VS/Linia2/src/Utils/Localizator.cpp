// 2023/08/08 15:52:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Localizator.h"
#include "Utils/Configurator.h"


wxString LL::ID(const wxString &id)
{
    wxString key = wxString::Format(id.c_str());

    Config::SetFile(wxGetCwd() + "/Linia.loc");

    wxString result = Config::ReadString(key);

    Config::SetFile("");

    return result;
}
