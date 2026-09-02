// 2026/05/05 15:08:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "Settings/Tests/SettingsTests.h"
#include "Settings/FileJSON.h"


// Класс для хранения набора тестов


class FileJSON;
class PanelCategory;


class Test
{
public:

    wxString _name{ "" };
};


class LibraryCategory
{
public:

    wxString name{ "" };
    wxString UGO{ "" };
    std::vector<Test> tests;
};


class Library
{
public:

    bool Read(FileJSON *);

    std::vector<LibraryCategory> categories;

private:

    FileJSON *file;

    // Заполняет library тестами из 
    // name_category - имя категория - BJT (транзистор), etc
    bool ParseCategory(LibraryCategory &library, const wxString &name_category, const rapidjson::Value &value);

    bool ParseNameCategory(LibraryCategory &, const wxString &name_category);

    bool ParseTest(LibraryCategory &, const rapidjson::Value &);
};
