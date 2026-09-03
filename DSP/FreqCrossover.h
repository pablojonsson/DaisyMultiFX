#pragma once
#ifndef FREQCROSSOVER_DSP
#define FREQCROSSOVER_DSP

#include <cmath>
#include "../DSP/FreqCrossover/Biquad.h"
#ifdef __cplusplus

namespace CustomDSP
{
class FreqCrossover
{
  public:
    void Init(float sample_rate, float crossover_freq);

    void Process(float inL, float inR, float &low_outL, float &high_outL, float &low_outR, float &high_outR);

    void SetCrossoverFreq(float freq);

  private:
    Biquad lowL1;
    Biquad lowL2;
    Biquad highL1;
    Biquad highL2;

    Biquad lowR1;
    Biquad lowR2;
    Biquad highR1;
    Biquad highR2;

    float crossover_frequency;

    float sampling_freq;
};
}
#endif
#endif