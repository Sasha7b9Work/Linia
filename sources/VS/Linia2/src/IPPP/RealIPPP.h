// 2025/09/10 21:27:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "IPPP/IPPP.h"


class RealIPPP : public IPPP
{
public:

    virtual void Init() override;

    virtual void Start() override;

    virtual void Stop() override;

    virtual bool ReadData(std::vector<int>(&data)[4]);

private:

    void Pause();

    virtual ~RealIPPP() { }
};
