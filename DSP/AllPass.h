#pragma once
#ifndef ALLPASS_DSP
#define ALLPASS_DSP

#include <daisysp.h>

#ifdef __cplusplus
namespace CustomDSP
{
class AllPass
{
  public:
    AllPass(){};
    ~AllPass(){};

    void Init(float sample_rate);

    float Process(float in);

    void Reset();

    float SetAlpha(float frequency);

  private:
    float sampling_freq;
    float last_input;
    float last_output;
    float alpha;
};
} // namespace CustomDSP

#endif
#endif