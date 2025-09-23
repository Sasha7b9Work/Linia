// 2025/09/10 21:27:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "IPPP/I_IPPP.h"


class RealIPPP : public I_IPPP
{
public:

    virtual void Init() override;

    virtual void Update() override;

    virtual void PressButtonStart() override;

    virtual void PressButtonStop() override;

private:

    virtual bool ReadData(std::vector<int>(&data)[4]);

    void Pause();

    virtual void SetPointCount(int) override;
};
