#pragma once
#ifndef CHORUS_EFFECT
#define CHORUS_EFFECT

#include "daisysp.h"
#include "daisy_seed.h"

#ifdef __cplusplus
namespace CustomEffects
{
class Chorus
{
  public:
    Chorus()
    {
    }
    ~Chorus()
    {
    }

    void Init(float sample_rate);

    void Process(float left_in, float right_in, float &left_out, float &right_out);

    void setRate(float freq);

    void setDepth(float amount);

    void SoftReset();
    void ClearStep();

  private:
    // sampling_freq: the sampling frequency of the audio processor
    // rate: the freq of the lfo in hZ .2-2.0
    // depth: how much the delay time moves around the center point
    // base_delay: the root delay time between the input and output in ms
    // lfo_phase: keeps track of where in the sin cycle the lfo is 0-2pi
    // phase_increment: how far each sample moves along the phase
    // mix: split between wet and dry signal. 0-1, 0 is dry, 1 is wet
    float sampling_freq, rate, depth, base_delay, lfo_phase, phase_increment, mix;
    // write_index:
    int write_index;

    bool clearing = false;
    int clear_samples_remaining = 0;

    float lfo_sin;
    float lfo_cos;

    float sin_increment;
    float cos_increment;
};
} // namespace CustomEffects
#endif
#endif