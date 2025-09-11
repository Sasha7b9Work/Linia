// 2025/08/19 14:10:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Tests/Tests.h"
#include "Panels/Panel03_Config/PanelScheme/PanelScheme.h"
#include "Utils/Configurator.h"
#include "Panels/Panel03_Config/Panel03_Config.h"


void Test::Load(pchar file_name)
{
    Config::SetFile(wxGetCwd() + "/" + file_name);

    PanelConfig::self->Unpack();

    Config::SetFile("");
}


void Test::Save(pchar file_name)
{
    Config::SetFile(wxGetCwd() + "/" + file_name);

    PanelConfig::self->Pack();

    Config::SetFile("");
}


