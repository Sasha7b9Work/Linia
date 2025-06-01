// 2023/09/01 12:08:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "DataBase/DataBasePasswords.h"
#include "Utils/Configurator.h"
#include "Utils/StringUtils.h"
#include "Settings/Settings.h"
#include "DataBase/DataBaseCards.h"
#include <cstdlib>
#include <chrono>
#include <vector>
#include <algorithm>
#include <string>


namespace PasswDB
{
    static std::vector<uint64> passwords;           // Здесь хранятся пароли - от первого применяемого к последнему

    static wxString file_name = "base.pass";

    static wxString NameFile()  { return wxGetCwd() + "/" + file_name; }

    void SetNameFile(const wxString &_file_name)
    {
        file_name = _file_name;
    }

    // Загрузить базу данных из файла
    static void Load();
}


void PasswDB::Init()
{
    Load();

    if (Count() == 0)
    {
        Append(0);

        DeInit();

        Load();
    }
}


void PasswDB::Load()
{
    if (!wxFileExists(NameFile()))
    {
        return;
    }

    passwords.clear();

    wxTextFile file;

    file.Open(NameFile());

    uint num_lines = file.GetLineCount();

    for (uint i = 0; i < num_lines; i++)
    {
        wxString line = file.GetLine(i);

        if (line.IsNumber())
        {
            uint64 password = 0;

            if (line.ToULongLong(&password))
            {
                passwords.push_back(password);
            }
        }
    }

    file.Close();
}


void PasswDB::DeInit()
{
    wxTextFile file;

    if (!wxFileExists(NameFile()))
    {
        file.Create(NameFile());
        file.Close();
    }

    if (file.Open(NameFile()))
    {
        file.Clear();

        for (uint i = 0; i < (passwords.size() > 10 ? 10 : passwords.size()); i++)
        {
            file.AddLine(wxString::Format("%llu", passwords[i]));
        }

        file.Write();

        file.Close();
    }
}


void PasswDB::Append(uint64 pass)
{
    auto it = std::find(passwords.begin(), passwords.end(), pass);

    if (it != passwords.end())
    {
        passwords.erase(it);
    }

    passwords.insert(passwords.begin(), pass);
}


int PasswDB::Count()
{
    return (int)passwords.size();
}


uint64 PasswDB::GetPassword(int i)
{
    return passwords[(uint)i];
}
