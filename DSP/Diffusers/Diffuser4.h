#pragma once
#ifndef DIFFUSER_4_DSP
#define DIFFUSER_4_DSP

#include "daisysp.h"

#ifdef __cplusplus
namespace CustomDSP
{
class Diffuser4
{
  public:
    Diffuser4(){};
    ~Diffuser4(){};

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
    daisysp::DelayLine<float, 74> delay_1;
    daisysp::DelayLine<float, 90> delay_2;
    daisysp::DelayLine<float, 108> delay_3;
    daisysp::DelayLine<float, 128> delay_4;
    daisysp::DelayLine<float, 150> delay_5;
    daisysp::DelayLine<float, 164> delay_6;
    daisysp::DelayLine<float, 182> delay_7;
    daisysp::DelayLine<float, 198> delay_8;
};
} // namespace CustomDSP
#endif
#endif