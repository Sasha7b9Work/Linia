// 2025/05/19 13:26:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/ConverterMP3.h"
#include "mpg123.h"
#include <fstream>


bool ConverterMP3::ReadFromFile(pchar filename, std::vector<int> &out)
{
    out.clear();

    mpg123_handle *mh = nullptr;
    int err = MPG123_OK;

    // Инициализация библиотеки
    if (mpg123_init() != MPG123_OK)
    {
        std::cerr << "mpg123 initialization failed" << std::endl;
        return false;
    }

    // Создание обработчика
    mh = mpg123_new(nullptr, &err);
    if (!mh)
    {
        std::cerr << "mpg123_new failed: " << mpg123_plain_strerror(err) << std::endl;
        mpg123_exit();
        return false;
    }

    // Открытие файла
    if (mpg123_open(mh, filename) != MPG123_OK)
    {
        std::cerr << "mpg123_open failed: " << mpg123_strerror(mh) << std::endl;
        mpg123_delete(mh);
        mpg123_exit();
        return false;
    }

    // Получение информации о аудио
    long rate;
    int channels, encoding;
    if (mpg123_getformat(mh, &rate, &channels, &encoding) != MPG123_OK)
    {
        std::cerr << "mpg123_getformat failed" << std::endl;
        mpg123_close(mh);
        mpg123_delete(mh);
        mpg123_exit();
        return false;
    }

    // Установка вывода в формате signed 16-bit
    mpg123_format_none(mh);
    mpg123_format(mh, rate, channels, MPG123_ENC_SIGNED_16);

    // Буфер для сэмплов
    size_t buffer_size = mpg123_outblock(mh);
    unsigned char *buffer = new unsigned char[buffer_size];

    // Чтение и сохранение сэмплов
    do
    {
        size_t done = 0;

        err = mpg123_read(mh, buffer, buffer_size, &done);

        if (err != MPG123_OK)
        {
            break;
        }

        if (done == 0)
        {
            break;
        }

        size_t samples_count = done / sizeof(short);
        short *samples = (short *)buffer;

        for (size_t i = 0; i < samples_count; i++)
        {
            out.push_back((int)samples[i]);
        }

    } while (true);

    // Очистка
    delete[] buffer;

    if (err != MPG123_DONE)
    {
        LOG_ERROR("Decoding failed: %s", mpg123_strerror(mh));
        mpg123_close(mh);
        mpg123_delete(mh);
        mpg123_exit();
        return false;
    }

    // Завершение работы
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();

    return true;
}


bool ConverterMP3::ReadFromFile(pchar filename, pchar output_name)
{
    std::vector<int> samples;

    if (ReadFromFile(filename, samples))
    {
        // Открытие выходного файла
        wxTextFile outFile;

        wxString full_name = wxGetCwd() + "/" + output_name;

        if (outFile.Open(full_name))
        {
            outFile.Clear();
        }
        else
        {
            outFile.Create(full_name);
        }

        if (!outFile.IsOpened())
        {
            std::cerr << "Failed to open output file" << std::endl;
            return false;
        }

        for (uint i = 0; i < samples.size(); i++)
        {
            outFile.AddLine(wxString::Format("%d", samples[i]));
        }

        outFile.Write();

        outFile.Close();

        return true;
    }

    return false;
}
