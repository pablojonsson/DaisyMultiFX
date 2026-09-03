#pragma once
#ifndef PEDAL_CONTROLS_H
#define PEDAL_CONTROLS_H

#include "daisy_seed.h"

enum class Effect
{
    Distortion,
    Overdrive,
    Chorus,
    Reverb,
    Phaser,
    Filter,
    Delay,
    None,
    Count
};

class PedalControls
{
  public:
    void Init(daisy::DaisySeed &hw);

    void Update();

    Effect GetEffect(int slot) const;

    float GetPot1(int slot) const;
    float GetPot2(int slot) const;

    int GetSelectedSlot() const;

    bool ConsumeResetRequest(int slot, Effect &old_effect);
    bool Pot1Changed(int slot);
    bool Pot2Changed(int slot);

  private:
    static constexpr float POT_DEADBAND = 0.005f;

    bool pot1_changed_1 = false;
    bool pot2_changed_1 = false;

    bool pot1_changed_2 = false;
    bool pot2_changed_2 = false;

    void HandleEncoder();
    void HandleSwitches();
    void UpdatePots();
    void UpdateLeds();
    float UpdatePotValue(float current, float new_value, bool &changed);

    Effect NextEffect(Effect current, Effect other);

    daisy::Color GetEffectColor(Effect effect);

    daisy::DaisySeed *hw;

    daisy::Encoder encoder;

    daisy::RgbLed led1;
    daisy::RgbLed led2;

    daisy::Switch switch1;
    daisy::Switch switch2;

    bool freeze_pots;

    int selected_slot;

    Effect curr_effect_1;
    Effect curr_effect_2;

    float pot1_value_1;
    float pot2_value_1;

    float pot1_value_2;
    float pot2_value_2;

    bool reset_pending_1;
    bool reset_pending_2;

    Effect reset_effect_1;
    Effect reset_effect_2;
};

#endif