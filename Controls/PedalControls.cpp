#include "PedalControls.h"
#include <cmath>

using namespace daisy;

void PedalControls::Init(DaisySeed &seed)
{
    hw = &seed;

    freeze_pots = true;

    selected_slot = 0;

    curr_effect_1 = Effect::Distortion;
    curr_effect_2 = Effect::None;

    pot1_value_1 = 0.0f;
    pot2_value_1 = 0.0f;

    pot1_value_2 = 0.0f;
    pot2_value_2 = 0.0f;

    reset_pending_1 = false;
    reset_pending_2 = false;

    reset_effect_1 = Effect::None;
    reset_effect_2 = Effect::None;

    encoder.Init(
        hw->GetPin(26),
        hw->GetPin(25),
        hw->GetPin(13));

    AdcChannelConfig pot1_config;
    pot1_config.InitSingle(hw->GetPin(21));

    AdcChannelConfig pot2_config;
    pot2_config.InitSingle(hw->GetPin(15));

    AdcChannelConfig pot_configs[] =
    {
        pot1_config,
        pot2_config
    };

    hw->adc.Init(pot_configs, 2);
    hw->adc.Start();

    led1.Init(
        hw->GetPin(20),
        hw->GetPin(19),
        hw->GetPin(18),
        true);

    led2.Init(
        hw->GetPin(17),
        hw->GetPin(24),
        hw->GetPin(23),
        true);

    switch1.Init(hw->GetPin(27));
    switch2.Init(hw->GetPin(28));

    led1.SetColor(GetEffectColor(curr_effect_1));
    led1.Update();

    led2.SetColor(GetEffectColor(curr_effect_2));
    led2.Update();

    hw->SetLed(freeze_pots);
}

void PedalControls::Update()
{
    HandleEncoder();
    HandleSwitches();
    UpdatePots();
    UpdateLeds();
}

void PedalControls::HandleEncoder()
{
    encoder.Debounce();

    if(encoder.RisingEdge())
    {
        freeze_pots = !freeze_pots;
        hw->SetLed(freeze_pots);
    }

    int new_index =
        selected_slot + encoder.Increment();

    int num_slots = 2;

    new_index =
        ((new_index % num_slots) + num_slots)
        % num_slots;

    selected_slot = new_index;
}

void PedalControls::HandleSwitches()
{
    switch1.Debounce();
    switch2.Debounce();

    if(switch1.RisingEdge())
    {
        Effect old = curr_effect_1;

        curr_effect_1 =
            NextEffect(
                curr_effect_1,
                curr_effect_2);

        if(old != curr_effect_1)
        {
            reset_effect_1 = old;
            reset_pending_1 = true;

            pot1_changed_1 = true;
            pot2_changed_1 = true;
        }
    }

    if(switch2.RisingEdge())
    {
        Effect old = curr_effect_2;

        curr_effect_2 =
            NextEffect(
                curr_effect_2,
                curr_effect_1);

        if(old != curr_effect_2)
        {
            reset_effect_2 = old;
            reset_pending_2 = true;

            pot1_changed_2 = true;
            pot2_changed_2 = true;
        }
    }
}

void PedalControls::UpdateLeds()
{
    Color color_1 =
        GetEffectColor(curr_effect_1);

    Color color_2 =
        GetEffectColor(curr_effect_2);

    if(selected_slot == 0)
    {
        color_2.Init(
            color_2.Red() * 0.5f,
            color_2.Green() * 0.5f,
            color_2.Blue() * 0.5f);
    }
    else
    {
        color_1.Init(
            color_1.Red() * 0.5f,
            color_1.Green() * 0.5f,
            color_1.Blue() * 0.5f);
    }

    led1.SetColor(color_1);
    led2.SetColor(color_2);

    led1.Update();
    led2.Update();
}

Effect PedalControls::NextEffect(
    Effect current,
    Effect other)
{
    int next =
        static_cast<int>(current);

    int count =
        static_cast<int>(Effect::Count);

    do
    {
        next = (next + 1) % count;
    }
    while(
        static_cast<Effect>(next) == other
        && other != Effect::None);

    return static_cast<Effect>(next);
}

Color PedalControls::GetEffectColor(
    Effect effect)
{
    Color color;

    switch(effect)
    {
        case Effect::Distortion:
            color.Init(
                1.0f,
                0.0f,
                0.0f);
            break;

        case Effect::Overdrive:
            color.Init(
                1.0f,
                1.0f,
                0.0f);
            break;

        case Effect::Chorus:
            color.Init(
                0.0f,
                0.0f,
                1.0f);
            break;

        case Effect::Reverb:
            color.Init(
                0.0f,
                1.0f,
                0.0f);
            break;

        case Effect::Phaser:
            color.Init(
                0.0f,
                1.0f,
                1.0f);
            break;

        case Effect::Filter:
            color.Init(
                1.0f,
                1.0f,
                1.0f);
            break;

        case Effect::Delay:
            color.Init(
                1.0f,
                0.0f,
                1.0f);
            break;

        case Effect::None:
        default:
            color.Init(
                0.0f,
                0.0f,
                0.0f);
            break;
    }

    return color;
}

Effect PedalControls::GetEffect(
    int slot) const
{
    if(slot == 0)
        return curr_effect_1;

    return curr_effect_2;
}

float PedalControls::GetPot1(
    int slot) const
{
    if(slot == 0)
        return pot1_value_1;

    return pot1_value_2;
}

float PedalControls::GetPot2(
    int slot) const
{
    if(slot == 0)
        return pot2_value_1;

    return pot2_value_2;
}

int PedalControls::GetSelectedSlot() const
{
    return selected_slot;
}

bool PedalControls::ConsumeResetRequest(
    int slot,
    Effect &old_effect)
{
    if(slot == 0)
    {
        if(!reset_pending_1)
            return false;

        old_effect = reset_effect_1;
        reset_pending_1 = false;

        return true;
    }

    if(!reset_pending_2)
        return false;

    old_effect = reset_effect_2;
    reset_pending_2 = false;

    return true;
}

float PedalControls::UpdatePotValue(float current,
                                    float new_value,
                                    bool &changed)
{
    if(fabsf(new_value - current) >= POT_DEADBAND)
    {
        changed = true;
        return new_value;
    }

    return current;
}

void PedalControls::UpdatePots()
{
    if(freeze_pots)
        return;

    float pot1 = hw->adc.GetFloat(0);
    float pot2 = hw->adc.GetFloat(1);

    if(selected_slot == 0)
    {
        pot1_value_1 =
            UpdatePotValue(
                pot1_value_1,
                pot1,
                pot1_changed_1);

        pot2_value_1 =
            UpdatePotValue(
                pot2_value_1,
                pot2,
                pot2_changed_1);
    }
    else
    {
        pot1_value_2 =
            UpdatePotValue(
                pot1_value_2,
                pot1,
                pot1_changed_2);

        pot2_value_2 =
            UpdatePotValue(
                pot2_value_2,
                pot2,
                pot2_changed_2);
    }
}

bool PedalControls::Pot1Changed(int slot)
{
    if(slot == 0)
    {
        bool changed = pot1_changed_1;
        pot1_changed_1 = false;
        return changed;
    }

    bool changed = pot1_changed_2;
    pot1_changed_2 = false;
    return changed;
}

bool PedalControls::Pot2Changed(int slot)
{
    if(slot == 0)
    {
        bool changed = pot2_changed_1;
        pot2_changed_1 = false;
        return changed;
    }

    bool changed = pot2_changed_2;
    pot2_changed_2 = false;
    return changed;
}