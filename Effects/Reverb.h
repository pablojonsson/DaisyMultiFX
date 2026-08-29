#pragma once
#ifndef REVERB_EFFECT
#define REVERB_EFFECT

#include "../DSP/MasterDiffuser.h"
#include "../DSP/ReverbFeedback.h"
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

    void Process(float inL, float inR, float &outL, float &outR);

    void setFeedback(float amount);

    void setMix(float amount);

    void setDamping(float amount);

  private:
    // sampling_frq: The sample rate
    // feedback: amount of reverb that feeds back into signal 0-1
    // mix: mix of dry and wet signal 0-1 dry-wet
    // damping: how fast the signal dies out 0-1
    float sampling_freq, feedback, mix, damping;

    CustomDSP::MasterDiffuser diffuser;

    CustomDSP::ReverbFeedback reverb_feedback;
};
} // namespace CustomEffects

#endif
#endif