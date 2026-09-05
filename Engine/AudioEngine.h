#pragma once

#ifndef AUDIOENGINE
#define AUDIOENGINE

#include "../DSP/StateVariableFilter.h"
#include "../Effects/Chorus.h"
#include "../Effects/Delay.h"
#include "../Effects/Distortion.h"
#include "../Effects/EffectTypes.h"
#include "../Effects/Overdrive.h"
#include "../Effects/Phaser.h"
#include "../Effects/Reverb.h"

class AudioEngine {
  public:
    static constexpr int NUM_EFFECT_SLOTS = 4;

    void Init(float sample_rate);

    void SetSlotEffect(int slot, Effect effect);

    void UpdateSlotParameters(int slot, float pot1, float pot2, bool pot1_changed,
                              bool pot2_changed);

    void Process(float inL, float inR, float &outL, float &outR);

    void ResetEffect(Effect effect);
    void ServiceInactiveEffects();

  private:
    CustomEffects::Distortion distortion;
    CustomEffects::Overdrive overdrive;
    CustomEffects::Delay delay;
    CustomEffects::Chorus chorus;
    CustomEffects::Reverb reverb;
    CustomEffects::Phaser phaser;
    CustomDSP::StateVariableFilter svf;
    void ProcessEffect(Effect effect, float inL, float inR, float &outL, float &outR);

    bool IsEffectActive(Effect effect) const;

    Effect slots[NUM_EFFECT_SLOTS];

    CustomDSP::StateVariableFilter filter;
};

#endif