#include "AudioEngine.h"

void AudioEngine::Init(float sample_rate) {
    distortion.Init(sample_rate);
    overdrive.Init(sample_rate);
    chorus.Init(sample_rate);
    reverb.Init(sample_rate);
    phaser.Init(sample_rate);
    delay.Init(sample_rate);
    svf.Init(sample_rate);
    svf.SetMode(CustomDSP::FilterMode::LowPass);
    for (int i = 0; i < NUM_EFFECT_SLOTS; i++)
        slots[i] = Effect::None;
}

void AudioEngine::SetSlotEffect(int slot, Effect effect) {
    if (slot < 0 || slot >= NUM_EFFECT_SLOTS)
        return;

    slots[slot] = effect;
}

bool AudioEngine::IsEffectActive(Effect effect) const {
    for (int i = 0; i < NUM_EFFECT_SLOTS; i++) {
        if (slots[i] == effect)
            return true;
    }

    return false;
}

void AudioEngine::ProcessEffect(Effect curr_effect, float inL, float inR, float &outL,
                                float &outR) {
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
void AudioEngine::Process(float inL, float inR, float &outL, float &outR) {
    float currentL = inL;
    float currentR = inR;

    for (int slot = 0; slot < NUM_EFFECT_SLOTS; slot++) {
        float nextL;
        float nextR;

        ProcessEffect(slots[slot], currentL, currentR, nextL, nextR);

        currentL = nextL;
        currentR = nextR;
    }

    outL = currentL;
    outR = currentR;
}

void AudioEngine::ResetEffect(Effect effect) {
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

void AudioEngine::ServiceInactiveEffects() {
    if (!IsEffectActive(Effect::Reverb))
        reverb.ClearStep();

    if (!IsEffectActive(Effect::Delay))
        delay.ClearStep();

    if (!IsEffectActive(Effect::Chorus))
        chorus.ClearStep();
}

void AudioEngine::UpdateSlotParameters(int slot, float pot1, float pot2, bool pot1_changed,
                                       bool pot2_changed) {
    Effect effect = slots[slot];

    switch (effect) {
    case Effect::Distortion: {
        if (pot1_changed) {
            float drive = 2.0f + 58.0f * (pot1 * pot1);

            distortion.SetDrive(drive);
        }

        if (pot2_changed)
            distortion.SetTone(pot2);

        break;
    }

    case Effect::Overdrive: {
        if (pot1_changed) {
            float drive = 1.0f + pot1 * 14.0f;

            overdrive.setDrive(drive);
        }

        if (pot2_changed)
            overdrive.setVolume(pot2);

        break;
    }

    case Effect::Chorus: {
        if (pot1_changed) {
            float shaped = pot1 * pot1;
            float rate = 0.2f + shaped * 14.8f;

            chorus.setRate(rate);
        }

        if (pot2_changed) {
            float depth = 0.1f + pot2 * 2.4f;

            chorus.setDepth(depth);
        }

        break;
    }

    case Effect::Reverb: {
        if (pot1_changed) {
            float shaped = pot1 * pot1;
            float decay_time = 0.3f + shaped * 19.7f;

            float x = -0.31084898f / decay_time;

            float x2 = x * x;
            float x3 = x2 * x;
            float x4 = x3 * x;
            float x5 = x4 * x;

            float feedback =
                1.0f + x + 0.5f * x2 + 0.16666667f * x3 + 0.04166667f * x4 + 0.008333333f * x5;

            reverb.setFeedback(feedback);
        }

        if (pot2_changed) {
            float damping = 0.45f - pot2 * 0.40f;

            reverb.setDamping(damping);
        }

        break;
    }

    case Effect::Phaser: {
        if (pot1_changed) {
            float rate = 0.05f * powf(100.0f, pot1);

            phaser.SetRate(rate);
        }

        if (pot2_changed)
            phaser.SetDepth(pot2);

        break;
    }

    case Effect::Filter: {
        if (pot1_changed) {
            float x2 = pot1 * pot1;
            float x4 = x2 * x2;

            float cutoff = 40.0f + x4 * 17960.0f;

            svf.SetCutoff(cutoff);
        }

        if (pot2_changed) {
            float shaped = pot2 * pot2;
            float q = 0.5f + shaped * 9.5f;

            svf.SetResonance(q);
        }

        break;
    }

    case Effect::Delay: {
        if (pot1_changed) {
            float shaped = pot1 * pot1;

            float delay_ms = 20.0f + shaped * 475.0f;

            delay.SetDelayTime(delay_ms);
        }

        if (pot2_changed) {
            float feedback = pot2 * 0.95f;

            delay.SetFeedback(feedback);
        }

        break;
    }

    case Effect::None:
    default:
        break;
    }
}