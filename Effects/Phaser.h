#pragma once
#ifndef PHASER_EFFECT
#define PHASER_EFFECT

#include "daisysp.h"

#ifdef __cplusplus
namespace CustomEffects
{

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
    float ProcessStage(AllPassStage &stage, float input, float coefficient);

    float sampling_freq;

    float lfo_phase = 0.0f;
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
#endif