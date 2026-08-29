#pragma once
#ifndef REVERB_FEEDBACK_DSP
#define REVERB_FEEDBACK_DSP

#include "./Diffusers/DiffuserMath.h"
#include "daisysp.h"
#include <array>

#ifdef __cplusplus
namespace CustomDSP
{
class ReverbFeedback
{
  public:
    ReverbFeedback()
    {
    }
    ~ReverbFeedback()
    {
    }

    void Init(float sample_rate);

    void Process(float inL, float inR, float &outL, float &outR);

    void setFeedback(float amount);

    void setDamping(float amount);

  private:
    // sampling_frq: The sample rate
    // feedback: amount of reverb that feeds back into signal 0-1
    // damping: how fast the signal dies out 0-1
    float sampling_freq, feedback, damping;

    float rate;  // Hz
    float depth; // samples

    float filter[8];

    float lfo_phase[8];
    float phase_increment[8];

    daisysp::DelayLine<float, 1500> delay_1;
    daisysp::DelayLine<float, 1700> delay_2;
    daisysp::DelayLine<float, 2000> delay_3;
    daisysp::DelayLine<float, 2200> delay_4;
    daisysp::DelayLine<float, 2450> delay_5;
    daisysp::DelayLine<float, 2650> delay_6;
    daisysp::DelayLine<float, 2850> delay_7;
    daisysp::DelayLine<float, 3100> delay_8;
};
} // namespace CustomDSP

#endif
#endif