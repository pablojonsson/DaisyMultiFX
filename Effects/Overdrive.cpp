#include "Overdrive.h"

using namespace CustomEffects;

void Overdrive::Init(float sample_rate) {
    sampling_freq = sample_rate;
    drive = .5f;
    volume = .7f;
    tone_cutoff = 20000;
}

float Overdrive::Process(float in) {
    float x = drive * in;

    if (x > 1.0f)
        x = 1.0f;
    else if (x < -1.0f)
        x = -1.0f;

    float out = x - (x * x * x) / 3.0f;

    return out * volume;
}

void Overdrive::setDrive(float amount) { drive = amount; }

void Overdrive::setVolume(float amount) { volume = amount; }