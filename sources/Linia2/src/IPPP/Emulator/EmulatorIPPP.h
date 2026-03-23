// 2025/09/10 21:36:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "IPPP/I_IPPP.h"


class EmulatorIPPP : public I_IPPP
{
public:

    void Update() override;

    void PressButtonStart() override;

    void PressButtonStop() override;

private:

    bool in_process_measuring = false;                  // Устанавливается при нажатии кнопки СТАРТ. Снимается при нажатии кнопки СТОП.

    virtual bool ReadData(
        std::array<int, 100> &data1,
        std::array<int, 100> &data2,
        std::array<int, 100> &data3,
        std::array<int, 100> &data4
    );
};
