// 2025/09/10 21:36:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "IPPP/I_IPPP.h"


class EmulatorIPPP : public I_IPPP
{
public:

    virtual void Init() override;

    virtual void Update() override;

    virtual void PressButtonStart() override;

    virtual void PressButtonStop() override;

private:

    bool in_process_measuring = false;                  // Устанавливается при нажатии кнопки СТАРТ. Снимается при нажатии кнопки СТОП.

    virtual bool ReadData(std::vector<int>(&data)[4]);

    virtual void SetPointCount(int) override;

    virtual void SetCircuitConnection(const Chan &, StateJack::E) override;

    virtual void SetTypeScan(TypeScan::E) override;

    virtual void SetPulseDuration(uint durationUS, bool generation_stump) override;

    virtual void SetFirstQueue(const Chan &) override;

    //------------------------------------------------------------------------------------------------------------

    virtual void ChanC_SourceRange(RangeU::E) override;

    virtual void ChanC_MeasRange(RangeU::E) override;
    virtual void ChanC_MeasRange(RangeI::E) override;

    virtual void ChanC_LimitSourceU(int min, int max) override;

    //------------------------------------------------------------------------------------------------------------

    virtual void ChanBS_SourceMode(const Chan &, ModeSource::E) override;
    virtual void ChanBS_StepRange(const Chan &, RangeU::E) override;
    virtual void ChanBS_StepRange(const Chan &, RangeI::E) override;
    virtual void ChanBS_StepCount(const Chan &, int) override;
    virtual void ChanBS_StepValue(const Chan &, double) override;
    virtual void ChaBS_Offset(const Chan &, double) override;
    virtual void ChanBS_MeasMode(const Chan &, ModeMeas::E) override;
    virtual void ChanBS_MeasRange(const Chan &, RangeU::E) override;
    virtual void ChanBS_MeasRange(const Chan &, RangeI::E) override;
    virtual void ChanBS_LimitRange(const Chan &, RangeU::E) override;
    virtual void ChanBS_LimitRange(const Chan &, RangeI::E) override;
    virtual void ChanBS_LimitThreshold(const Chan &, int) override;
};
