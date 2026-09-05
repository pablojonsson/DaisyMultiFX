#pragma once
#ifndef PEDAL_CONTROLS_H
#define PEDAL_CONTROLS_H

#include "../Effects/EffectTypes.h"
#include "daisy_seed.h"

class PedalControls {
  public:
    void Init(daisy::DaisySeed &hw);

    void Update();

    Effect GetEffect(int slot) const;

    float GetPot1(int slot) const;
    float GetPot2(int slot) const;

    int GetSelectedSlot() const;

    void SaveCurrentSlot();

    bool ConsumeResetRequest(int slot, Effect &old_effect);
    bool Pot1Changed(int slot);
    bool Pot2Changed(int slot);

    int GetCurrentPage() const;

  private:
    static constexpr float POT_DEADBAND = 0.005f;
    static constexpr float POT_PICKUP_THRESHOLD = 0.02f;
    static constexpr int NUM_EFFECT_SLOTS = 4;

    void HandleEncoder();
    void HandleSwitches();
    void UpdatePots();
    void UpdateLeds();
    float UpdatePotValue(float current, float new_value, bool &changed);
    void CycleSlot(int slot);
    void ClearSlot(int slot);
    void UpdateSaveLed();

    Effect NextEffect(Effect current, int slot);

    daisy::Color GetEffectColor(Effect effect);

    daisy::DaisySeed *hw;

    daisy::Encoder encoder;

    daisy::RgbLed led1;
    daisy::RgbLed led2;

    daisy::Switch switch1;
    daisy::Switch switch2;

    int selected_slot;

    static constexpr uint32_t LONG_PRESS_MS = 750;

    static constexpr uint32_t SAVE_HOLD_MS = 750;

    static constexpr uint32_t SAVE_FLASH_MS = 150;

    bool encoder_save_triggered = false;

    bool save_led_flash = false;
    uint32_t save_led_flash_start = 0;

    bool switch1_long_press = false;
    bool switch2_long_press = false;

    int switch1_slot = 0;
    int switch2_slot = 1;

    Effect curr_effect[NUM_EFFECT_SLOTS];

    float pot1_value[NUM_EFFECT_SLOTS];
    float pot2_value[NUM_EFFECT_SLOTS];

    float saved_pot1_value[NUM_EFFECT_SLOTS];
    float saved_pot2_value[NUM_EFFECT_SLOTS];

    bool pot1_picked_up[NUM_EFFECT_SLOTS];
    bool pot2_picked_up[NUM_EFFECT_SLOTS];

    bool pot1_changed[NUM_EFFECT_SLOTS];
    bool pot2_changed[NUM_EFFECT_SLOTS];

    bool reset_pending[NUM_EFFECT_SLOTS];
    Effect reset_effect[NUM_EFFECT_SLOTS];
};

#endif