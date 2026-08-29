#pragma once
#ifndef MASTER_DIFFUSER_DSP
#define MASTER_DIFFUSER_DSP

#include "../DSP/Diffusers/Diffuser16.h"
#include "../DSP/Diffusers/Diffuser2.h"
#include "../DSP/Diffusers/Diffuser4.h"
#include "../DSP/Diffusers/Diffuser8.h"

#ifdef __cplusplus
namespace CustomDSP
{
class MasterDiffuser
{
  public:
    MasterDiffuser(){};
    ~MasterDiffuser(){};

    void Init(float sample_rate);

    void Process(float inL, float inR, float &outL, float &outR);

  private:
    float sampling_freq;
    Diffuser2 diff2;
    Diffuser4 diff4;
    Diffuser8 diff8;
    Diffuser16 diff16;
};
} // namespace CustomDSP
#endif
#endif