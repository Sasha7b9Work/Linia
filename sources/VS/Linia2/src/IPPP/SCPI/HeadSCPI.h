// 2025/10/23 12:29:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace SCPI
{
    struct StructSCPI
    {
        pchar begin;            // С этим сравниваем начало передаваемой строки
        bool (*func)(pchar);    // А эта функция вызывается в случае, если начало совпадает
        StructSCPI *handler;    // Или передаётся в эту последовательность структур

        static bool Update(pchar, StructSCPI *);
    };

    extern StructSCPI head[];
}
