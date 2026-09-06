#pragma once

#ifndef AUDIOENGINE
#define AUDIOENGINE

#include "../DSP/FreqCrossover.h"
#include "../DSP/StateVariableFilter.h"
#include "../Effects/Chorus.h"
#include "../Effects/Delay.h"
#include "../Effects/Distortion.h"
#include "../Effects/EffectTypes.h"
#include "../Effects/Overdrive.h"
#include "../Effects/Phaser.h"
#include "../Effects/Reverb.h"
#include "RoutingMode.h"

class AudioEngine
{
  public:
    static constexpr int NUM_EFFECT_SLOTS = 4;

    void Init(float sample_rate);

    void SetSlotEffect(int slot, Effect effect);

    void UpdateSlotParameters(int slot, float pot1, float pot2, bool pot1_changed,
                              bool pot2_changed);

    void Process(float inL, float inR, float &outL, float &outR);
    void ProcessSeries(float inL, float inR, float &outL, float &outR);
    void ProcessSplit(float inL, float inR, float &outL, float &outR);

    void ResetEffect(Effect effect);
    void ServiceInactiveEffects();

    void SetRoutingMode(RoutingMode mode);

    void SetSplitPosition(int position);
    void SetJoinPosition(int position);

    void SetSlotBranch(int slot, Branch branch);

    void SetSplitFrequency(float frequency);

  private:
    CustomEffects::Distortion distortion;
    CustomEffects::Overdrive overdrive;
    CustomEffects::Delay delay;
    CustomEffects::Chorus chorus;
    CustomEffects::Reverb reverb;
    CustomEffects::Phaser phaser;
    CustomDSP::StateVariableFilter svf;
    CustomDSP::StateVariableFilter filter;
    CustomDSP::FreqCrossover crossover;

    RoutingMode routing_mode;

    int split_position;
    int join_position;

    void ProcessEffect(Effect effect, float inL, float inR, float &outL, float &outR);

    bool IsEffectActive(Effect effect) const;

    Effect slots[NUM_EFFECT_SLOTS];
    Branch slot_branch[NUM_EFFECT_SLOTS];
};

#endif