#include "PedalControls.h"
#include <cmath>

using namespace daisy;

void PedalControls::Init(DaisySeed &seed) {
    hw = &seed;

    selected_slot = 0;

    encoder_save_triggered = false;
    save_led_flash = false;

    hw->SetLed(false);

    for (int i = 0; i < NUM_EFFECT_SLOTS; ++i) {
        curr_effect[i] = Effect::None;

        pot1_value[i] = 0.0f;
        pot2_value[i] = 0.0f;

        saved_pot1_value[i] = 0.0f;
        saved_pot2_value[i] = 0.0f;

        pot1_changed[i] = false;
        pot2_changed[i] = false;

        pot1_picked_up[i] = false;
        pot2_picked_up[i] = false;

        reset_pending[i] = false;
        reset_effect[i] = Effect::None;
    }

    encoder.Init(hw->GetPin(26), hw->GetPin(25), hw->GetPin(13));

    AdcChannelConfig pot1_config;
    pot1_config.InitSingle(hw->GetPin(21));

    AdcChannelConfig pot2_config;
    pot2_config.InitSingle(hw->GetPin(15));

    AdcChannelConfig pot_configs[] = {pot1_config, pot2_config};

    hw->adc.Init(pot_configs, 2);
    hw->adc.Start();

    led1.Init(hw->GetPin(20), hw->GetPin(19), hw->GetPin(18), true);

    led2.Init(hw->GetPin(17), hw->GetPin(24), hw->GetPin(23), true);

    switch1.Init(hw->GetPin(27));
    switch2.Init(hw->GetPin(28));

    led1.SetColor(GetEffectColor(curr_effect[0]));
    led1.Update();

    led2.SetColor(GetEffectColor(curr_effect[1]));
    led2.Update();
}

void PedalControls::Update() {
    HandleEncoder();
    HandleSwitches();
    UpdatePots();
    UpdateLeds();
    UpdateSaveLed();
}

void PedalControls::UpdateLeds() {
    int first_slot = GetCurrentPage() * 2;
    int second_slot = first_slot + 1;

    Color color_1 = GetEffectColor(curr_effect[first_slot]);
    Color color_2 = GetEffectColor(curr_effect[second_slot]);

    if (curr_effect[first_slot] == Effect::None) {
        if (selected_slot == first_slot)
            color_1.Init(0.15f, 0.15f, 0.15f);
        else
            color_1.Init(0.0f, 0.0f, 0.0f);
    }

    if (curr_effect[second_slot] == Effect::None) {
        if (selected_slot == second_slot)
            color_2.Init(0.15f, 0.15f, 0.15f);
        else
            color_2.Init(0.0f, 0.0f, 0.0f);
    }

    if (selected_slot == first_slot) {
        color_2.Init(color_2.Red() * 0.5f, color_2.Green() * 0.5f, color_2.Blue() * 0.5f);
    } else {
        color_1.Init(color_1.Red() * 0.5f, color_1.Green() * 0.5f, color_1.Blue() * 0.5f);
    }

    led1.SetColor(color_1);
    led2.SetColor(color_2);

    led1.Update();
    led2.Update();
}

void PedalControls::CycleSlot(int slot) {
    Effect old = curr_effect[slot];

    curr_effect[slot] = NextEffect(curr_effect[slot], slot);

    if (old != curr_effect[slot]) {
        reset_effect[slot] = old;
        reset_pending[slot] = true;

        pot1_changed[slot] = true;
        pot2_changed[slot] = true;
    }
}

void PedalControls::ClearSlot(int slot) {
    Effect old = curr_effect[slot];

    if (old == Effect::None)
        return;

    curr_effect[slot] = Effect::None;

    reset_effect[slot] = old;
    reset_pending[slot] = true;
}

void PedalControls::HandleEncoder() {
    encoder.Debounce();

    int old_slot = selected_slot;

    int increment = encoder.Increment();

    if (increment != 0) {
        selected_slot += increment;

        if (selected_slot < 0)
            selected_slot = 0;

        if (selected_slot >= NUM_EFFECT_SLOTS)
            selected_slot = NUM_EFFECT_SLOTS - 1;
    }

    if (selected_slot != old_slot) {
        pot1_value[old_slot] = saved_pot1_value[old_slot];
        pot2_value[old_slot] = saved_pot2_value[old_slot];

        pot1_changed[old_slot] = true;
        pot2_changed[old_slot] = true;

        pot1_picked_up[selected_slot] = false;
        pot2_picked_up[selected_slot] = false;
    }

    if (encoder.RisingEdge())
        encoder_save_triggered = false;

    if (!encoder_save_triggered && encoder.TimeHeldMs() >= SAVE_HOLD_MS) {
        SaveCurrentSlot();
        encoder_save_triggered = true;
    }
}

void PedalControls::SaveCurrentSlot() {
    saved_pot1_value[selected_slot] = pot1_value[selected_slot];

    saved_pot2_value[selected_slot] = pot2_value[selected_slot];

    save_led_flash = true;
    save_led_flash_start = System::GetNow();

    hw->SetLed(true);
}

void PedalControls::HandleSwitches() {
    switch1.Debounce();
    switch2.Debounce();

    int first_slot = GetCurrentPage() * 2;
    int second_slot = first_slot + 1;

    if (switch1.RisingEdge()) {
        switch1_slot = first_slot;
        switch1_long_press = false;
    }

    if (!switch1_long_press && switch1.TimeHeldMs() >= LONG_PRESS_MS) {
        ClearSlot(switch1_slot);
        switch1_long_press = true;
    }

    if (switch1.FallingEdge()) {
        if (!switch1_long_press)
            CycleSlot(switch1_slot);
    }

    if (switch2.RisingEdge()) {
        switch2_slot = second_slot;
        switch2_long_press = false;
    }

    if (!switch2_long_press && switch2.TimeHeldMs() >= LONG_PRESS_MS) {
        ClearSlot(switch2_slot);
        switch2_long_press = true;
    }

    if (switch2.FallingEdge()) {
        if (!switch2_long_press)
            CycleSlot(switch2_slot);
    }
}
void PedalControls::UpdatePots() {
    float pot1 = hw->adc.GetFloat(0);
    float pot2 = hw->adc.GetFloat(1);

    int slot = selected_slot;

    if (!pot1_picked_up[slot]) {
        if (fabsf(pot1 - pot1_value[slot]) <= POT_PICKUP_THRESHOLD) {
            pot1_picked_up[slot] = true;
        }
    } else {
        pot1_value[slot] = UpdatePotValue(pot1_value[slot], pot1, pot1_changed[slot]);
    }

    if (!pot2_picked_up[slot]) {
        if (fabsf(pot2 - pot2_value[slot]) <= POT_PICKUP_THRESHOLD) {
            pot2_picked_up[slot] = true;
        }
    } else {
        pot2_value[slot] = UpdatePotValue(pot2_value[slot], pot2, pot2_changed[slot]);
    }
}

Effect PedalControls::NextEffect(Effect current, int slot) {
    int next = static_cast<int>(current);
    int count = static_cast<int>(Effect::Count);

    while (true) {
        next = (next + 1) % count;

        Effect candidate = static_cast<Effect>(next);

        if (candidate == Effect::None)
            return candidate;

        bool already_used = false;

        for (int i = 0; i < NUM_EFFECT_SLOTS; i++) {
            if (i != slot && curr_effect[i] == candidate) {
                already_used = true;
                break;
            }
        }

        if (!already_used)
            return candidate;
    }
}

Color PedalControls::GetEffectColor(Effect effect) {
    Color color;

    switch (effect) {
    case Effect::Distortion:
        color.Init(1.0f, 0.0f, 0.0f);
        break;

    case Effect::Overdrive:
        color.Init(1.0f, 1.0f, 0.0f);
        break;

    case Effect::Chorus:
        color.Init(0.0f, 0.0f, 1.0f);
        break;

    case Effect::Reverb:
        color.Init(0.0f, 1.0f, 0.0f);
        break;

    case Effect::Phaser:
        color.Init(0.0f, 1.0f, 1.0f);
        break;

    case Effect::Filter:
        color.Init(1.0f, 1.0f, 1.0f);
        break;

    case Effect::Delay:
        color.Init(1.0f, 0.0f, 1.0f);
        break;

    case Effect::None:
    default:
        color.Init(0.0f, 0.0f, 0.0f);
        break;
    }

    return color;
}

Effect PedalControls::GetEffect(int slot) const { return curr_effect[slot]; }

float PedalControls::GetPot1(int slot) const { return pot1_value[slot]; }

float PedalControls::GetPot2(int slot) const { return pot2_value[slot]; }

int PedalControls::GetSelectedSlot() const { return selected_slot; }

bool PedalControls::ConsumeResetRequest(int slot, Effect &old_effect) {
    if (!reset_pending[slot])
        return false;

    old_effect = reset_effect[slot];
    reset_pending[slot] = false;

    return true;
}

float PedalControls::UpdatePotValue(float current, float new_value, bool &changed) {
    if (fabsf(new_value - current) >= POT_DEADBAND) {
        changed = true;
        return new_value;
    }

    return current;
}

bool PedalControls::Pot1Changed(int slot) {
    bool changed = pot1_changed[slot];
    pot1_changed[slot] = false;
    return changed;
}

bool PedalControls::Pot2Changed(int slot) {
    bool changed = pot2_changed[slot];
    pot2_changed[slot] = false;
    return changed;
}

int PedalControls::GetCurrentPage() const { return selected_slot / 2; }

void PedalControls::UpdateSaveLed() {
    if (!save_led_flash)
        return;

    if (System::GetNow() - save_led_flash_start >= SAVE_FLASH_MS) {
        hw->SetLed(false);
        save_led_flash = false;
    }
}