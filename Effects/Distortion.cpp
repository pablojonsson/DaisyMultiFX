#include "Distortion.h"

using namespace CustomEffects;

void Distortion::Init(float sample_rate)
{
    sampling_freq = sample_rate;
    drive = .5f;
    volume = .7f;
    tone_cutoff = 20000;
}

float Distortion::Process(float in)
{
    float x = drive * in;

    if (x > 1.0f)
        x = 1.0f;
    else if (x < -1.0f)
        x = -1.0f;

    float out = x - (x * x * x) / 3.0f;

    return out * volume;
}

void Distortion::setDrive(float amount)
{
    drive = amount;
}

void Distortion::setVolume(float amount)
{
    volume = amount;
}