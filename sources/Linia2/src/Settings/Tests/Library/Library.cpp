// 2026/05/05 15:11:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Settings/Tests/Library/Library.h"


bool Library::Read(FileJSON *_file)
{
    bool exist_errors = false;

    using namespace rapidjson;

    file = _file;

    const Document &doc = file->Document();

    for (auto it = doc.MemberBegin(); it != doc.MemberEnd(); ++it)
    {
        if (it->value.IsObject())
        {
            LibraryCategory *library = new LibraryCategory();

            if (!wxString(it->name.GetString()).StartsWith("REM"))              // С последовательности "REM" начинаются комментарии. Пропускаем
            {
                if (ParseCategory(library, it->name.GetString(), it->value))
                {
                    categories.push_back(library);
                }
                else
                {
                    exist_errors = true;
                }
            }
        }
    }

    if (exist_errors)
    {
        LOG_ERROR("Errors occurred while parsing the file %s", _file->GetFullPath().c_str());
    }
    else
    {
        LOG_WRITE("Load library from %s is OK!", _file->GetFullPath().c_str());
    }

    return true;
}


bool Library::ParseCategory(LibraryCategory *library, const wxString &name_category, const rapidjson::Value &value)
{
    for (auto it_value = value.MemberBegin(); it_value != value.MemberEnd(); ++it_value)
    {
        if (it_value->value.IsString())
        {
            library->UGO = name_category;

            if (!ParseNameCategory(library, name_category))
            {
                LOG_ERROR("Can not parse category %s", name_category.c_str().AsChar());

                return false;
            }
        }
        else if (it_value->value.IsObject())
        {
            Test *test = new Test{ library };

            test->UGO = wxString::FromUTF8(it_value->name.GetString());

            if (ParseTest(test, it_value->value))
            {
                library->tests.push_back(test);
            }
            else
            {
                return false;
            }
        }
    }

    return true;
}


bool Library::ParseNameCategory(LibraryCategory *library, const wxString &name_category)
{
    library->name = wxString::FromUTF8(file->GetStringValue(name_category, "name"));

    return library->name.Length() != 0;
}


bool Library::ParseTest(Test *test, const rapidjson::Value &value)
{
    for (auto it = value.MemberBegin(); it != value.MemberEnd(); ++it)
    {
        if (it->value.IsString())
        {
            if (wxString(it->name.GetString()) == "name")
            {
                test->name = wxString::FromUTF8(it->value.GetString());
            }
        }
        else if (it->value.IsObject())
        {

        }
    }

    return test->name[0] != '\0';
}
