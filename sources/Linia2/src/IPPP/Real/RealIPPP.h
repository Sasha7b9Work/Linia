// 2025/09/10 21:27:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "IPPP/I_IPPP.h"


class RealIPPP : public I_IPPP
{
public:

    void Update() override;

    void PressButtonStart() override;

    void PressButtonStop() override;

private:

    bool IsChanBS(const Chan &) const;

    virtual bool ReadData(
        std::array<int, 100> &data1,
        std::array<int, 100> &data2,
        std::array<int, 100> &data3,
        std::array<int, 100> &data4
    );

    void Pause();
};
