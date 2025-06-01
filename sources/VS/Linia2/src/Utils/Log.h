// 2023/08/31 13:22:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define LOG_WRITE(...)          Log::WriteTrace(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_WRITE_SERVER(...)   Log::WriteServerTrace(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
//#define LOG_WRITE_TRACE(...)  Log::WriteTrace(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

#define LOG_ERROR(...)          Log::ErrorTrace(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_ERROR_SERVER(...)   Log::ErrorServerTrace(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
//#define LOG_ERROR_TRACE(...)  Log::ErrorTrace(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)


namespace Log
{
    void Init();

    void FromDevice(char *);

    void ToDevice(char *);

    void Write(char *file, int line, char *format, ...);

    void Error(char *file, int line, char *format, ...);

    void WriteTrace(const char *file, int line, const char *function, const char *format, ...);

    void WriteServerTrace(const char *file, int line, const char *function, const char *format, ...);

    void ErrorTrace(const char *file, int line, const char *function, const char *format, ...);

    void ErrorServerTrace(const char *file, int line, const char *function, const char *format, ...);
}
