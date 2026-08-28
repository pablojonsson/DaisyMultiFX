#include "Distortion.h"
#include <cmath>

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
    float out;
    out = (tanh(drive * in));
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