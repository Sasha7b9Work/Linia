// 2026/05/05 15:08:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "Settings/Tests/SettingsTests.h"
#include "Settings/FileJSON.h"


class LibraryCategory;


// Класс для хранения набора тестов


class Test
{
public:

    static Test *current;       // Текущий тест

    LibraryCategory *lib;       // В состав данной категории входит данный тест
    wxString UGO{ "" };         // УГО теста
    wxString name{ "" };        // Имя теста

    bool IsBJT() const;         // Биполярный транзистор
    bool IsBJTS() const;        // Биполярный транзистор с подложкой
    bool IsFET() const;         // Полевой транзистор
    bool IsFETS() const;        // Полевой транзистор с подложкой
    bool IsDARL() const;
    bool IsTHYR() const;        // Тиристор
    bool IsDIOD() const;
    bool IsZENER() const;       // Стабилитрон
    bool IsRES() const;
    bool IsCAP() const;         // Конденсатор
};


// Здесь тесты, относящиеся к данной категории
class LibraryCategory
{
public:

    wxString name{ "" };
    wxString UGO{ "" };
    std::vector<Test *> tests;
};


class Library
{
public:

    bool Read(FileJSON *);

    std::vector<LibraryCategory *> categories;

private:

    FileJSON *file;

    // Заполняет library тестами из 
    // name_category - имя категория - BJT (транзистор), etc
    bool ParseCategory(LibraryCategory *library, const wxString &name_category, const rapidjson::Value &value);

    bool ParseNameCategory(LibraryCategory *, const wxString &name_category);

    bool ParseTest(Test *, const rapidjson::Value &);
};
