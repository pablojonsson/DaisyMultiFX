#include "DSP/StateVariableFilter.h"
#include "Effects/Chorus.h"
#include "Effects/Delay.h"
#include "Effects/Overdrive.h"
#include "Effects/Reverb.h"
#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;
Encoder encoder;
RgbLed led1;
RgbLed led2;
Switch switch1;
Switch switch2;

CustomEffects::Overdrive overdrive;
CustomEffects::Delay delay;
CustomEffects::Chorus chorus;
CustomEffects::Reverb reverb;
CustomDSP::StateVariableFilter svf;

bool freeze_pots = true;

enum class Effect
{
    Overdrive,
    Chorus,
    Reverb,
    Filter,
    Delay,
    None,
    Count
};

Effect curr_effect_1 = Effect::Overdrive;
Effect curr_effect_2 = Effect::None;
int selected_effect = 0;

float pot1_value_1 = 0.0f;
float pot2_value_1 = 0.0f;

float pot1_value_2 = 0.0f;
float pot2_value_2 = 0.0f;

Effect NextEffect(Effect current, Effect other)
{
    int next = static_cast<int>(current);
    int count = static_cast<int>(Effect::Count);

    do
    {
        next = (next + 1) % count;
    } while (static_cast<Effect>(next) == other && other != Effect::None);

    return static_cast<Effect>(next);
}

Color GetEffectColor(Effect effect)
{
    Color color;
    switch (effect)
    {
    case Effect::Overdrive:
        color.Init(1.0f, 0.0f, 0.0f);
        break;

    case Effect::Chorus:
        color.Init(0.0f, 0.0f, 1.0f);
        break;

    case Effect::Reverb:
        color.Init(0.0f, 1.0f, 0.0f);
        break;

    case Effect::Filter:
        color.Init(1.0f, 1.0f, 1.0f);
        break;
    case Effect::Delay:
        color.Init(1.0f, 0.0f, 1.0f);
        break;
    default:
        color.Init(0.0f, 0.0f, 0.0f);
        break;
    }
    return color;
}

void ResetEffect(Effect effect)
{
    switch (effect)
    {
    case Effect::Chorus:
        chorus.Reset();
        break;

    case Effect::Reverb:
        reverb.Reset();
        break;

    case Effect::Filter:
        svf.Reset();
        break;

    case Effect::Delay:
        delay.Reset();
        break;

    default:
        break;
    }
}

void HandleEncoder()
{
    encoder.Debounce();

    if (encoder.RisingEdge())
    {
        freeze_pots = !freeze_pots;
        hw.SetLed(freeze_pots);
    }

    int newIndex = selected_effect + encoder.Increment();

    int numEffects = 2;

    newIndex = ((newIndex % numEffects) + numEffects) % numEffects;

    selected_effect = newIndex;
}

void HandleSwitches()
{
    switch1.Debounce();
    switch2.Debounce();

    if (switch1.RisingEdge())
    {
        Effect old = curr_effect_1;

        curr_effect_1 = NextEffect(curr_effect_1, curr_effect_2);

        if (old != curr_effect_1)
            ResetEffect(old);
    }
    if (switch2.RisingEdge())
    {
        Effect old = curr_effect_2;

        curr_effect_2 = NextEffect(curr_effect_2, curr_effect_1);

        if (old != curr_effect_2)
            ResetEffect(old);
    }
}

void UpdateLeds()
{
    Color color_1 = GetEffectColor(curr_effect_1);
    Color color_2 = GetEffectColor(curr_effect_2);

    if (selected_effect == 0)
    {
        color_2.Init(color_2.Red() * .5, color_2.Green() * .5, color_2.Blue() * .5);
    }
    if (selected_effect == 1)
    {
        color_1.Init(color_1.Red() * .5, color_1.Green() * .5, color_1.Blue() * .5);
    }
    led1.SetColor(color_1);
    led2.SetColor(color_2);
    led1.Update();
    led2.Update();
}

void Update()
{
    HandleEncoder();
    HandleSwitches();
    UpdateLeds();
}

void UpdateEffectParameters(Effect curr_effect, int effect_slot)
{
    float pot1_value;
    float pot2_value;

    if (effect_slot == 0)
    {
        if (!freeze_pots && selected_effect == 0)
        {
            pot1_value_1 = hw.adc.GetFloat(0);
            pot2_value_1 = hw.adc.GetFloat(1);
        }

        pot1_value = pot1_value_1;
        pot2_value = pot2_value_1;
    }
    else
    {
        if (!freeze_pots && selected_effect == 1)
        {
            pot1_value_2 = hw.adc.GetFloat(0);
            pot2_value_2 = hw.adc.GetFloat(1);
        }

        pot1_value = pot1_value_2;
        pot2_value = pot2_value_2;
    }

    switch (curr_effect)
    {
    case Effect::Overdrive:
    {
        float drive = 1.0f + pot1_value * 14.0f;

        overdrive.setDrive(drive);
        overdrive.setVolume(pot2_value);

        break;
    }

    case Effect::Chorus:
    {
        float shaped = pot1_value * pot1_value;

        float rate = 0.2f + shaped * 14.8f;
        float depth = 0.1f + pot2_value * 2.4f;

        chorus.setRate(rate);
        chorus.setDepth(depth);

        break;
    }

    case Effect::Reverb:
    {
        float shaped = pot1_value * pot1_value;

        float decay_time = 0.3f + shaped * 19.7f;
        float damping = 0.45f - pot2_value * 0.40f;

        float x = -0.31084898f / decay_time;

        float x2 = x * x;
        float x3 = x2 * x;
        float x4 = x3 * x;
        float x5 = x4 * x;

        float feedback =
            1.0f + x + 0.5f * x2 + 0.16666667f * x3 + 0.04166667f * x4 + 0.008333333f * x5;

        reverb.setFeedback(feedback);
        reverb.setDamping(damping);

        break;
    }

    case Effect::Filter:
    {
        float x2 = pot1_value * pot1_value;
        float x4 = x2 * x2;

        float cutoff = 40.0f + x4 * 17960.0f;

        float resonance_shape = pot2_value * pot2_value;
        float q = 0.5f + resonance_shape * 9.5f;

        svf.SetCutoff(cutoff);
        svf.SetResonance(q);

        break;
    }

    case Effect::Delay:
    {
        float shaped = pot1_value * pot1_value;

        float delay_ms = 20.0f + shaped * 475.0f;
        float feedback = pot2_value * 0.95f;

        delay.SetDelayTime(delay_ms);
        delay.SetFeedback(feedback);

        break;
    }

    default:
        break;
    }
}

void HandleCurrEffect(Effect curr_effect, float inL, float inR, float &outL, float &outR)
{
    switch (curr_effect)
    {
    case Effect::Overdrive:
    {
        outL = overdrive.Process(inL);
        outR = overdrive.Process(inR);
        break;
    }

    case Effect::Chorus:
    {
        chorus.Process(inL, inR, outL, outR);
        break;
    }

    case Effect::Reverb:
    {
        reverb.Process(inL, inR, outL, outR);
        break;
    }

    case Effect::Filter:
    {
        svf.Process(inL, inR, outL, outR);
        break;
    }

    case Effect::Delay:
    {
        delay.Process(inL, inR, outL, outR);
        break;
    }

    case Effect::None:
    default:
    {
        outL = inL;
        outR = inR;
        break;
    }
    }
}

void ApplyEffects(AudioHandle::InputBuffer &in, AudioHandle::OutputBuffer &out, size_t size)
{
    UpdateEffectParameters(curr_effect_1, 0);
    UpdateEffectParameters(curr_effect_2, 1);

    for (size_t i = 0; i < size; i++)
    {
        float stage1L;
        float stage1R;

        HandleCurrEffect(curr_effect_1, in[0][i], in[1][i], stage1L, stage1R);

        HandleCurrEffect(curr_effect_2, stage1L, stage1R, out[0][i], out[1][i]);
    }
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    Update();
    ApplyEffects(in, out, size);
}

void InitControls()
{
    encoder.Init(hw.GetPin(26), hw.GetPin(25), hw.GetPin(13));

    AdcChannelConfig pot1_config;
    pot1_config.InitSingle(hw.GetPin(21));

    AdcChannelConfig pot2_config;
    pot2_config.InitSingle(hw.GetPin(15));

    AdcChannelConfig pot_configs[] = {pot1_config, pot2_config};

    hw.adc.Init(pot_configs, 2);
    hw.adc.Start();

    led1.Init(hw.GetPin(20), hw.GetPin(19), hw.GetPin(18), true);

    led2.Init(hw.GetPin(17), hw.GetPin(24), hw.GetPin(23), true);

    switch1.Init(hw.GetPin(27));
    switch2.Init(hw.GetPin(28));

    led1.SetColor(GetEffectColor(curr_effect_1));
    led1.Update();

    led2.SetColor(GetEffectColor(curr_effect_2));
    led2.Update();

    hw.SetLed(freeze_pots);
}

void InitEffects()
{
    overdrive.Init(hw.AudioSampleRate());
    chorus.Init(hw.AudioSampleRate());
    reverb.Init(hw.AudioSampleRate());
    svf.Init(hw.AudioSampleRate());
    delay.Init(hw.AudioSampleRate());
}

int main(void)
{
    hw.Init();

    hw.SetAudioBlockSize(4);
    hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

    InitControls();

    InitEffects();

    hw.StartAudio(AudioCallback);

    while (1)
    {
    }
}