#include "Controls/PedalControls.h"
#include "DSP/FreqCrossover.h"
#include "DSP/StateVariableFilter.h"
#include "Effects/Chorus.h"
#include "Effects/Delay.h"
#include "Effects/Distortion.h"
#include "Effects/Overdrive.h"
#include "Effects/Phaser.h"
#include "Effects/Reverb.h"

#include "daisy_seed.h"
#include "daisysp.h"

#include <cmath>

using namespace daisy;
using namespace daisysp;

CpuLoadMeter loadMeter;

DaisySeed hw;

PedalControls controls;

CustomEffects::Distortion distortion;
CustomEffects::Overdrive overdrive;
CustomEffects::Delay delay;
CustomEffects::Chorus chorus;
CustomEffects::Reverb reverb;
CustomEffects::Phaser phaser;
CustomDSP::StateVariableFilter svf;
CustomDSP::FreqCrossover crossover;

static constexpr int NUM_EFFECT_SLOTS = 4;

void ResetEffect(Effect effect) {
    switch (effect) {
    case Effect::Chorus:
        chorus.SoftReset();
        break;

    case Effect::Reverb:
        reverb.SoftReset();
        break;

    case Effect::Filter:
        svf.Reset();
        break;

    case Effect::Delay:
        delay.SoftReset();
        break;

    case Effect::Phaser:
        phaser.Reset();
        break;

    default:
        break;
    }
}

bool IsEffectActive(Effect effect) {
    for (int slot = 0; slot < NUM_EFFECT_SLOTS; slot++) {
        if (controls.GetEffect(slot) == effect)
            return true;
    }

    return false;
}

void UpdateEffectParameters(Effect curr_effect, int effect_slot) {
    float pot1_value = controls.GetPot1(effect_slot);
    float pot2_value = controls.GetPot2(effect_slot);

    bool pot1_changed = controls.Pot1Changed(effect_slot);
    bool pot2_changed = controls.Pot2Changed(effect_slot);

    switch (curr_effect) {
    case Effect::Overdrive: {
        if (!pot1_changed && !pot2_changed)
            break;
        float drive = 1.0f + pot1_value * 14.0f;

        overdrive.setDrive(drive);
        overdrive.setVolume(pot2_value);

        break;
    }

    case Effect::Distortion: {
        if (!pot1_changed && !pot2_changed)
            break;
        float shaped = pot1_value * pot1_value;

        float drive = 1.0f + shaped * 29.0f;

        distortion.SetDrive(drive);
        distortion.SetTone(pot2_value);

        break;
    }

    case Effect::Chorus: {
        if (!pot1_changed && !pot2_changed)
            break;
        float shaped = pot1_value * pot1_value;

        float rate = 0.2f + shaped * 14.8f;
        float depth = 0.1f + pot2_value * 2.4f;

        chorus.setRate(rate);
        chorus.setDepth(depth);

        break;
    }

    case Effect::Reverb: {
        if (!pot1_changed && !pot2_changed)
            break;
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

    case Effect::Filter: {
        if (!pot1_changed && !pot2_changed)
            break;
        float x2 = pot1_value * pot1_value;
        float x4 = x2 * x2;

        float cutoff = 40.0f + x4 * 17960.0f;

        float resonance_shape = pot2_value * pot2_value;
        float q = 0.5f + resonance_shape * 9.5f;

        svf.SetCutoff(cutoff);
        svf.SetResonance(q);

        break;
    }

    case Effect::Delay: {
        if (!pot1_changed && !pot2_changed)
            break;
        float shaped = pot1_value * pot1_value;

        float delay_ms = 20.0f + shaped * 475.0f;
        float feedback = pot2_value * 0.95f;

        delay.SetDelayTime(delay_ms);
        delay.SetFeedback(feedback);

        break;
    }

    case Effect::Phaser: {
        if (!pot1_changed && !pot2_changed)
            break;
        float rate = 0.05f * powf(100.0f, pot1_value);
        float depth = pot2_value;

        phaser.SetRate(rate);
        phaser.SetDepth(depth);

        break;
    }

    case Effect::None:
    default:
        break;
    }
}

void HandleCurrEffect(Effect curr_effect, float inL, float inR, float &outL, float &outR) {
    switch (curr_effect) {
    case Effect::Overdrive: {
        outL = overdrive.Process(inL);
        outR = overdrive.Process(inR);

        break;
    }

    case Effect::Distortion: {
        outL = distortion.Process(inL);
        outR = distortion.Process(inR);

        break;
    }

    case Effect::Chorus: {
        chorus.Process(inL, inR, outL, outR);
        break;
    }

    case Effect::Reverb: {
        reverb.Process(inL, inR, outL, outR);
        break;
    }

    case Effect::Filter: {
        svf.Process(inL, inR, outL, outR);
        break;
    }

    case Effect::Delay: {
        delay.Process(inL, inR, outL, outR);
        break;
    }

    case Effect::Phaser: {
        phaser.Process(inL, inR, outL, outR);
        break;
    }

    case Effect::None:
    default: {
        outL = inL;
        outR = inR;
        break;
    }
    }
}

void ServiceInactiveEffects() {
    if (!IsEffectActive(Effect::Reverb))
        reverb.ClearStep();

    if (!IsEffectActive(Effect::Delay))
        delay.ClearStep();

    if (!IsEffectActive(Effect::Chorus))
        chorus.ClearStep();
}

void ApplyEffects(AudioHandle::InputBuffer &in, AudioHandle::OutputBuffer &out, size_t size) {
    for (int slot = 0; slot < NUM_EFFECT_SLOTS; slot++) {
        UpdateEffectParameters(controls.GetEffect(slot), slot);
    }

    for (size_t i = 0; i < size; i++) {
        float currentL = in[0][i];
        float currentR = in[1][i];

        for (int slot = 0; slot < NUM_EFFECT_SLOTS; slot++) {
            float nextL;
            float nextR;

            HandleCurrEffect(controls.GetEffect(slot), currentL, currentR, nextL, nextR);

            currentL = nextL;
            currentR = nextR;
        }

        out[0][i] = currentL;
        out[1][i] = currentR;
    }

    ServiceInactiveEffects();
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) {
    loadMeter.OnBlockStart();

    controls.Update();

    Effect old_effect;

    for (int slot = 0; slot < NUM_EFFECT_SLOTS; slot++) {
        if (controls.ConsumeResetRequest(slot, old_effect))
            ResetEffect(old_effect);
    }

    ApplyEffects(in, out, size);

    loadMeter.OnBlockEnd();
}

void InitEffects() {
    float sample_rate = hw.AudioSampleRate();

    overdrive.Init(sample_rate);
    distortion.Init(sample_rate);
    chorus.Init(sample_rate);
    reverb.Init(sample_rate);
    svf.Init(sample_rate);
    delay.Init(sample_rate);
    phaser.Init(sample_rate);
    crossover.Init(sample_rate, 1000.0f);
}

int main(void) {
    hw.Init();

    hw.SetAudioBlockSize(16);
    hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

    loadMeter.Init(hw.AudioSampleRate(), hw.AudioBlockSize());

    hw.StartLog();

    controls.Init(hw);

    InitEffects();

    hw.StartAudio(AudioCallback);

    while (1) {
        float avg = loadMeter.GetAvgCpuLoad() * 100.0f;
        float max = loadMeter.GetMaxCpuLoad() * 100.0f;

        hw.PrintLine("CPU:");
        hw.PrintLine("AVG: " FLT_FMT3, FLT_VAR3(avg));
        hw.PrintLine("MAX: " FLT_FMT3, FLT_VAR3(max));

        loadMeter.Reset();

        System::Delay(500);
    }
}