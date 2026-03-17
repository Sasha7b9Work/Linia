// 2026/03/17 17:32:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Value.h"
#include "Settings/Settings.h"


ValueBool::ValueBool(const wxString &_key, const bool &_def) : Value<bool>(_key, _def)
{
    SET::AppendValue(this);
}


ValueInt::ValueInt(const wxString &_key, const int &_def) : Value<int>(_key, _def)
{
    SET::AppendValue(this);
}


ValueUInt::ValueUInt(const wxString &_key, const uint &_def) : Value<uint>(_key, _def)
{
    SET::AppendValue(this);
}


ValuePoint::ValuePoint(const wxString &_key, const wxPoint &_def) : Value<wxPoint>(_key, _def)
{
    SET::AppendValue(this);
}
