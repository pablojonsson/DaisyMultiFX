#include "Controls/PedalControls.h"
#include "Engine/AudioEngine.h"

#include "daisy_seed.h"
#include "daisysp.h"

#include <cmath>

using namespace daisy;
using namespace daisysp;

DaisySeed hw;
AudioEngine engine;
PedalControls controls;

static constexpr int NUM_EFFECT_SLOTS = 4;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) {

    controls.Update();

    for (int slot = 0; slot < AudioEngine::NUM_EFFECT_SLOTS; slot++) {
        Effect effect = controls.GetEffect(slot);

        engine.SetSlotEffect(slot, effect);

        engine.UpdateSlotParameters(slot, controls.GetPot1(slot), controls.GetPot2(slot),
                                    controls.Pot1Changed(slot), controls.Pot2Changed(slot));
    }

    Effect old_effect;

    for (int slot = 0; slot < AudioEngine::NUM_EFFECT_SLOTS; slot++) {
        if (controls.ConsumeResetRequest(slot, old_effect)) {
            engine.ResetEffect(old_effect);
        }
    }

    for (size_t i = 0; i < size; i++) {
        engine.Process(in[0][i], in[1][i], out[0][i], out[1][i]);
    }

    engine.ServiceInactiveEffects();
}

int main(void) {
    hw.Init();

    hw.SetAudioBlockSize(16);
    hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

    controls.Init(hw);

    engine.Init(hw.AudioSampleRate());

    hw.StartAudio(AudioCallback);

    while (1) {
    }
}