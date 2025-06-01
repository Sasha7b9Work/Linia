// 2023/10/01 22:27:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/EventsLog.h"


void EventsLog::Append(const CardInfo &info)
{
    wxTextFile file(wxGetCwd() + "/events.log");

    if (!file.Exists())
    {
        file.Create();

        file.AddLine("Date              ID             Number");

        file.Write();

        file.Close();
    }

    file.Open();

    wxString line = wxString::Format("%s %s %llu", info.time, info.id, info.number);

    file.AddLine(line);

    file.Write();

    file.Close();
}
