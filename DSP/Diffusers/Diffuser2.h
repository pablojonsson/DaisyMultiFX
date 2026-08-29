#pragma once
#ifndef DIFFUSER_2_DSP
#define DIFFUSER_2_DSP

#include "./DiffuserMath.h"
#include "daisysp.h"

#ifdef __cplusplus
namespace CustomDSP
{
class Diffuser2
{
  public:
    Diffuser2(){};
    ~Diffuser2(){};

    void Init(float sample_rate);

    void Process(float inL, float inR, float &outL, float &outR);

    void Reset();

  private:
    float sampling_freq;
    float feedback = 0.5f;
    daisysp::DelayLine<float, 32> delay_1;
    daisysp::DelayLine<float, 44> delay_2;
    daisysp::DelayLine<float, 54> delay_3;
    daisysp::DelayLine<float, 62> delay_4;
    daisysp::DelayLine<float, 72> delay_5;
    daisysp::DelayLine<float, 80> delay_6;
    daisysp::DelayLine<float, 90> delay_7;
    daisysp::DelayLine<float, 98> delay_8;
};
} // namespace CustomDSP
#endif
#endif