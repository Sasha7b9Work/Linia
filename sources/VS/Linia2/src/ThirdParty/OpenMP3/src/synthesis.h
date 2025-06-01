#pragma once

#include "types.h"




//
//decl

namespace OpenMP3
{

	void Antialias(const FrameData & data, UInt gr, UInt ch, Float32 is[576]);

	void HybridSynthesis(const FrameData & data, UInt gr, UInt ch, Float32 store[32][18], Float32 is[576]);

	inline void FrequencyInversion(Float32 is[576])
	{
        for (UInt sb = 1; sb < 32; sb += 2) for (UInt i = 1; i < 18; i += 2) is[sb * 18 + i] = -is[sb * 18 + i];
	}

	void SubbandSynthesis(const FrameData & data, const Float32 is[576], Float32 v_vec[1024], Float32 output[576]);

}