// 2025/09/10 21:36:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/EmulatorIPPP.h"
#include "Application.h"


void EmulatorIPPP::Init()
{

}


void EmulatorIPPP::Update()
{
    if (in_process_measuring)
    {
        std::vector<int> data[4];

        if (ReadData(data))
        {
            Application::self->OnReadData(data);
        }
    }
}


void EmulatorIPPP::PressButtonStart()
{
    in_process_measuring = true;
}


void EmulatorIPPP::PressButtonStop()
{
    in_process_measuring = false;
}


bool EmulatorIPPP::ReadData(std::vector<int>(& /*data*/)[4])
{
    return false;
}


void EmulatorIPPP::SetPointCount(int)
{
    LOG_ERROR("Not implemented");
}


void EmulatorIPPP::SetCircuitConnection(const Chan &, StateJack::E)
{
    LOG_ERROR("Not implemented");
}


void EmulatorIPPP::SetTypeScan(TypeScan::E)
{
    LOG_ERROR("Not implemented");
}


void EmulatorIPPP::ChanC_SourceRange(RangeU::E)
{
    LOG_ERROR("Not implemented");
}

void EmulatorIPPP::ChanC_MeasRange(RangeU::E)
{
    LOG_ERROR("Not implemented");
}


void EmulatorIPPP::ChanC_MeasRange(RangeI::E)
{
    LOG_ERROR("Not implemented");
}


void EmulatorIPPP::ChanC_LimitSourceU(int /*min*/, int /*max*/)
{
    LOG_ERROR("Not implemented");
}


void EmulatorIPPP::ChanBS_SourceMode(const Chan &/*chan*/, ModeSource::E /*mode*/)
{
    LOG_ERROR("Not implemented");
}

void EmulatorIPPP::ChanBS_StepRange(const Chan &/*chan*/, RangeU::E /*range*/)
{
    LOG_ERROR("Not implemented");
}

void EmulatorIPPP::ChanBS_StepRange(const Chan &, RangeI::E)
{
    LOG_ERROR("Not implemented");
}

void EmulatorIPPP::ChanBS_StepCount(const Chan &/*chan*/, int /*count*/)
{
    LOG_ERROR("Not implemented");
}

void EmulatorIPPP::ChanBS_StepValue(const Chan &/*chan*/, double /*value*/)
{
    LOG_ERROR("Not implemented");
}

void EmulatorIPPP::ChaBS_Offset(const Chan &/*chan*/, double)
{
    LOG_ERROR("Not implemented");
}

void EmulatorIPPP::ChanBS_MeasMode(const Chan &/*chan*/, ModeMeas::E /*mode*/)
{
    LOG_ERROR("Not implemented");
}

void EmulatorIPPP::ChanBS_MeasRange(const Chan &/*chan*/, RangeU::E /*range*/)
{
    LOG_ERROR("Not implemented");
}

void EmulatorIPPP::ChanBS_MeasRange(const Chan &/*chan*/, RangeI::E /*range*/)
{
    LOG_ERROR("Not implemented");
}

void EmulatorIPPP::ChanBS_LimitRange(const Chan &/*chan*/, RangeU::E /*range*/)
{
    LOG_ERROR("Not implemented");
}


void EmulatorIPPP::ChanBS_LimitRange(const Chan &/*chan*/, RangeI::E /*range*/)
{
    LOG_ERROR("Not implemented");
}


void EmulatorIPPP::ChanBS_LimitThreshold(const Chan &/*chan*/, int /*threshold*/)
{
    LOG_ERROR("Not implemented");
}
