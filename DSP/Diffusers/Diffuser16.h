#pragma once
#ifndef DIFFUSER_16_DSP
#define DIFFUSER_16_DSP

#include "daisysp.h"

#ifdef __cplusplus
namespace CustomDSP
{
class Diffuser16
{
  public:
    Diffuser16(){};
    ~Diffuser16(){};

    void Init(float sample_rate);

    void Process(float inL, float inR, float &outL, float &outR);

  private:
    void Duplicate(float in);

    void Shuffle();

    void Hadamard();

    float signal_1;
    float signal_2;
    float signal_3;
    float signal_4;
    float signal_5;
    float signal_6;
    float signal_7;
    float signal_8;
    float sampling_freq;
    float feedback = 0.5f;
    daisysp::DelayLine<float, 282> delay_1;
    daisysp::DelayLine<float, 332> delay_2;
    daisysp::DelayLine<float, 390> delay_3;
    daisysp::DelayLine<float, 458> delay_4;
    daisysp::DelayLine<float, 524> delay_5;
    daisysp::DelayLine<float, 602> delay_6;
    daisysp::DelayLine<float, 674> delay_7;
    daisysp::DelayLine<float, 744> delay_8;
};
} // namespace CustomDSP
#endif
#endif