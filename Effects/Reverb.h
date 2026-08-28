#pragma once
#ifndef REVERB_EFFECT
#define REVERB_EFFECT

#include "../DSP/AllPass.h"
#include "daisysp.h"

#ifdef __cplusplus
namespace CustomEffects
{
class Reverb
{
  public:
    Reverb()
    {
    }
    ~Reverb()
    {
    }

    void Init(float sample_rate);

    float Process(float in);

    void setFeedback(float amount);

    void setMix(float amount);

    void setDamping(float amount);

  private:
    // sampling_frq: The sample rate
    // feedback: amount of reverb that feeds back into signal 0-1
    // mix: mix of dry and wet signal 0-1 dry-wet
    // damping: how fast the signal dies out 0-1
    float sampling_freq, feedback, mix, damping;

    float filter_state_1, filter_state_2, filter_state_3, filter_state_4;

    // Delay buffers for the reverb
    daisysp::DelayLine<float, 1600> delay_1;
    daisysp::DelayLine<float, 2000> delay_2;
    daisysp::DelayLine<float, 2600> delay_3;
    daisysp::DelayLine<float, 3200> delay_4;
    daisysp::DelayLine<float, 2048> pre_delay;

    // AllPass filter instances to smooth audio
    CustomDSP::AllPass allpass_1;
    CustomDSP::AllPass allpass_2;

    // lfo vars to subtly change delay time
    float lfo_phase = 0.0f;
    float lfo_rate = 0.2f;  // Hz
    float lfo_depth = 5.0f; // samples
    float lfo_increment = 0.0f;
};
} // namespace CustomEffects

#endif
#endif