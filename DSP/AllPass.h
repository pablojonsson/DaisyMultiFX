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

    void Init(float delay_samples, float feedback);

    float Process(float in);

  private:
    daisysp::DelayLine<float, 1024> delay;
    float feedback;
};
} // namespace CustomDSP

#endif
#endif