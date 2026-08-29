#pragma once
#ifndef DELAY_EFFECT
#define DELAY_EFFECT

#include "daisysp.h"

#ifdef __cplusplus
namespace CustomEffects
{
class Delay
{
  public:
    void Init(float sample_rate);
    void Process(float inL, float inR, float &outL, float &outR);

    void SetDelayTime(float time_ms);
    void SetFeedback(float feedback);
    void SetMix(float mix);

    void Reset();

  private:
    float sampling_freq;
    float target_feedback;
    float feedback;
    float mix;

    float target_delay;
    float current_delay;

    daisysp::DelayLine<float, 24000> delayL;
    daisysp::DelayLine<float, 24000> delayR;
};
} // namespace CustomEffects
#endif
#endif