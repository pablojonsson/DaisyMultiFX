#pragma once
#ifndef DIFFUSER_8_DSP
#define DIFFUSER_8_DSP

#include "./DiffuserMath.h"
#include "daisysp.h"

#ifdef __cplusplus
namespace CustomDSP
{
class Diffuser8
{
  public:
    Diffuser8(){};
    ~Diffuser8(){};

    void Init(float sample_rate);

    void Process(float inL, float inR, float &outL, float &outR);

    void Reset();

  private:
    float sampling_freq;
    float feedback = 0.5f;
    daisysp::DelayLine<float, 152> delay_1;
    daisysp::DelayLine<float, 180> delay_2;
    daisysp::DelayLine<float, 212> delay_3;
    daisysp::DelayLine<float, 240> delay_4;
    daisysp::DelayLine<float, 278> delay_5;
    daisysp::DelayLine<float, 314> delay_6;
    daisysp::DelayLine<float, 348> delay_7;
    daisysp::DelayLine<float, 384> delay_8;
};
} // namespace CustomDSP
#endif
#endif