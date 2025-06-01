// 2025/05/19 13:26:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace ConverterMP3
{
    // Считывает файл mp3 в массив значений. Если в файле 2 канала, то в массив ложится первый
    bool ReadFromFile(pchar name, std::vector<int> &);

    bool ReadFromFile(pchar input_name, pchar output_name);
}
