#pragma once
#ifndef PHASER_EFFECT
#define PHASER_EFFECT

#include "daisysp.h"

namespace CustomEffects
{

struct AllPassStage
{
    float previous_input  = 0.0f;
    float previous_output = 0.0f;

    float Process(float in, float a)
    {
        float out = -a * in + previous_input + a * previous_output;

        previous_input  = in;
        previous_output = out;

        return out;
    }

    void Reset()
    {
        previous_input  = 0.0f;
        previous_output = 0.0f;
    }
};

class Phaser
{
  public:
    void Init(float sample_rate);

    void Process(float inL, float inR, float &outL, float &outR);

    void SetRate(float rate);
    void SetDepth(float depth);
    void SetFeedback(float feedback);
    void SetMix(float mix);

    void Reset();

  private:
    float FrequencyToCoefficient(float frequency);

    static constexpr int CONTROL_DIV = 16;

    int control_counter = 0;

    float aL1 = 0.0f;
    float aL2 = 0.0f;
    float aL3 = 0.0f;
    float aL4 = 0.0f;

    float aR1 = 0.0f;
    float aR2 = 0.0f;
    float aR3 = 0.0f;
    float aR4 = 0.0f;

    float sampling_freq;

    float lfo_phase       = 0.0f;
    float phase_increment = 0.0f;

    float rate;
    float depth;
    float feedback;
    float mix;

    float feedbackL = 0.0f;
    float feedbackR = 0.0f;

    AllPassStage stagesL[4];
    AllPassStage stagesR[4];
};

} // namespace CustomEffects

#endif