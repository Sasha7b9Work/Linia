// 2023/08/11 17:01:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Configurator.h"


void Config::SetFile(const wxString &file_path)
{
    static wxString current_file = "unimaginable name";

    if (current_file == file_path)
    {
        return;
    }

    current_file = file_path;

    static wxFileConfig *config = nullptr;

    if (config)
    {
        SAFE_DELETE(config);
    }

    if (file_path.IsEmpty())
    {
        wxConfigBase::Set(g_file_config);
        g_config = wxConfigBase::Get(false);
    }
    else
    {
        config = new wxFileConfig("", "", file_path);

        wxConfigBase::Set(config);
        g_config = wxConfigBase::Get(false);
    }
}


int Config::ReadInt(const wxString &key, int def)
{
    int result = def;

    g_config->Read(key, &result, def);

    return result;
}


int Config::Read(const wxString &key, int def)
{
    return ReadInt(key, def);
}


wxPoint Config::Read(const wxString &key, const wxPoint &def)
{
    int x = ReadInt(key + "_x", def.x);
    int y = ReadInt(key + "_y", def.y);

    return { x, y };
}


void Config::Write(const wxString &key, const wxPoint &value)
{
    WriteInt(key + "_x", value.x);
    WriteInt(key + "_y", value.y);
}


uint64 Config::ReadUInt64(const wxString &key, uint64 def)
{
    uint64 result = def;

    g_config->Read(key, (int64 *)&result, (int64)def);

    return result;
}


BitSet128 Config::ReadUInt128(const wxString &key, const BitSet128 &def)
{
    BitSet128 result = def;

    g_config->Read(key + "[0]", (int64 *)&result.word64[0], (int64)def.word64[0]);
    g_config->Read(key + "[1]", (int64 *)&result.word64[1], (int64)def.word64[1]);

    return result;
}


int64 Config::ReadInt64(const wxString &key, int64 def)
{
    int64 result = def;

    g_config->Read(key, &result, def);

    return result;
}


bool Config::ReadBool(const wxString &key, bool def)
{
    bool result = def;

    g_config->Read(key, &result, def);

    return result;
}


bool Config::Read(const wxString &key, bool def)
{
    return ReadBool(key, def);
}


wxString Config::ReadString(const wxString &key, const wxString &def)
{
    wxString result = def;

    g_config->Read(key, &result, def);

    return result;
}


wxString Config::Read(const wxString &key, const wxString &def)
{
    return ReadString(key, def);
}


void Config::WriteInt(const wxString &key, int value)
{
    g_config->Write(key, value);
}


void Config::Write(const wxString &key, int value)
{
    WriteInt(key, value);
}


void Config::WriteUint(const wxString &key, uint value)
{
    g_config->Write(key, value);
}


void Config::Write(const wxString &key, const Color &value)
{
    WriteUint(key, value.value);
}


Color Config::Read(const wxString &key, const Color &def)
{
    Color result;
    result.value = (uint)ReadUInt64(key, def.value);
    return result;
}


void Config::WriteUInt64(const wxString &key, uint64 value)
{
    g_config->Write(key, (int64)value);
}


void Config::WriteUInt128(const wxString &key, const BitSet128 &value)
{
    g_config->Write(key + "[0]", (int64)value.word64[0]);
    g_config->Write(key + "[1]", (int64)value.word64[1]);
}


void Config::WriteInt64(const wxString &key, int64 value)
{
    g_config->Write(key, value);
}


void Config::WriteBool(const wxString &key, bool value)
{
    g_config->Write(key, value);
}


void Config::WriteString(const wxString &key, const wxString &value)
{
    g_config->Write(key, value);
}


void Config::Write(const wxString &key, const wxString &value)
{
    WriteString(key, value);
}
