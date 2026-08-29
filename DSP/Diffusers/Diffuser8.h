#pragma once
#ifndef DIFFUSER_8_DSP
#define DIFFUSER_8_DSP

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