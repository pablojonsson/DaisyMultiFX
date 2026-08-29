#include "DSP/AllPass.h"
#include "Effects/Chorus.h"
#include "Effects/Distortion.h"
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

CustomEffects::Distortion distortion;
CustomEffects::Chorus chorus;
CustomEffects::Reverb reverb;

bool stopAudio = true;

float targetVolume = 0.0f;
float volume = 0.7f;

enum class Effect
{
    Distortion,
    Chorus,
    Reverb,
    Filter,
    Count
};

Effect currEffect = Effect::Distortion;

Color::PresetColor GetEffectColor(Effect effect)
{
    switch (effect)
    {
    case Effect::Distortion:
        return Color::PresetColor::RED;

    case Effect::Chorus:
        return Color::PresetColor::BLUE;

    case Effect::Reverb:
        return Color::PresetColor::GREEN;

    case Effect::Filter:
        return Color::PresetColor::WHITE;

    default:
        return Color::PresetColor::RED;
    }
}

void HandleEncoder()
{
    encoder.Debounce();

    if (encoder.RisingEdge())
    {
        stopAudio = !stopAudio;
    }

    int numEffects = static_cast<int>(Effect::Count);

    int currentIndex = static_cast<int>(currEffect);

    int newIndex = currentIndex + encoder.Increment();

    newIndex = ((newIndex % numEffects) + numEffects) % numEffects;

    currEffect = static_cast<Effect>(newIndex);

    Color c;
    c.Init(GetEffectColor(currEffect));
    led2.SetColor(c);
}

void HandleSwitches()
{
    switch1.Debounce();
    switch2.Debounce();

    if (switch1.FallingEdge())
    {
        volume -= 0.1f;
    }

    if (switch2.FallingEdge())
    {
        volume += 0.1f;
    }

    if (volume > 1.3f)
    {
        volume = 1.3f;
    }

    if (volume < 0.0f)
    {
        volume = 0.0f;
    }
}

void UpdateLeds()
{
    if (stopAudio || volume <= 0.05f)
    {
        targetVolume = 0.0f;

        Color c;
        c.Init(Color::PresetColor::RED);
        led1.SetColor(c);
    }
    else
    {
        targetVolume = 1.0f;

        Color c;
        c.Init(Color::PresetColor::BLUE);
        led1.SetColor(c);
    }

    led1.Update();
    led2.Update();
}

void Update()
{
    HandleEncoder();
    UpdateLeds();
    HandleSwitches();
}

void ApplyEffect(AudioHandle::InputBuffer &in, AudioHandle::OutputBuffer &out, size_t size)
{
    float pot1_value = hw.adc.GetFloat(0);
    float pot2_value = hw.adc.GetFloat(1);

    switch (currEffect)
    {
    case Effect::Distortion:
    {
        float drive = 1.0f + pot1_value * 14.0f;

        distortion.setDrive(drive);
        distortion.setVolume(pot2_value);

        break;
    }

    case Effect::Chorus:
    {
        // float rate = 0.2f * powf(15.0f, pot1_value);
        float shaped = pot1_value * pot1_value;
        float rate = 0.2f + shaped * 14.8f;

        float depth = 0.1f + pot2_value * 2.4f;

        chorus.setRate(rate);
        chorus.setDepth(depth);

        break;
    }

    case Effect::Reverb:
    {
        // float decay_time = 0.3f * powf(20.0f, pot1_value);

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

    default:
        break;
    }
    for (size_t i = 0; i < size; i++)
    {
        switch (currEffect)
        {
        case Effect::Distortion:
        {
            out[0][i] = distortion.Process(in[0][i]);
            out[1][i] = distortion.Process(in[1][i]);

            break;
        }

        case Effect::Chorus:
        {
            chorus.Process(in[0][i], in[1][i], out[0][i], out[1][i]);

            break;
        }

        case Effect::Reverb:
        {
            reverb.Process(in[0][i], in[1][i], out[0][i], out[1][i]);

            break;
        }

        default:
        {
            out[0][i] = in[0][i];
            out[1][i] = in[1][i];

            break;
        }
        }
        out[0][i] *= volume * targetVolume;
        out[1][i] *= volume * targetVolume;
    }
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    Update();
    ApplyEffect(in, out, size);
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

    Color effectColor;
    effectColor.Init(GetEffectColor(currEffect));
    led2.SetColor(effectColor);
    led2.Update();
}

void InitEffects()
{
    distortion.Init(hw.AudioSampleRate());
    chorus.Init(hw.AudioSampleRate());
    reverb.Init(hw.AudioSampleRate());
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