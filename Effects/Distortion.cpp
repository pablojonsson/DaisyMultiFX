#include "Distortion.h"

using namespace CustomEffects;

void Distortion::Init(float sample_rate)
{
    sampling_freq = sample_rate;

    input_hpf.Init(sample_rate);
    input_hpf.SetMode(CustomDSP::FilterMode::HighPass);
    input_hpf.SetCutoff(100.0f);

    tone_lpf.Init(sample_rate);
    tone_lpf.SetMode(CustomDSP::FilterMode::LowPass);
    tone_lpf.SetCutoff(1800.0f);

    tone_hpf.Init(sample_rate);
    tone_hpf.SetMode(CustomDSP::FilterMode::HighPass);
    tone_hpf.SetCutoff(1800.0f);

    drive = 0.5f;
    tone = 0.5f;
}

float Distortion::Process(float in)
{
    float x;
    float dummy;

    input_hpf.Process(in, in, x, dummy);

    x *= drive;

    float distorted = x / (1.0f + fabsf(x));

    float toned;
    tone_lpf.Process(distorted, distorted, toned, dummy);
    
    return toned * 0.7f;;
}

void Distortion::SetDrive(float amount)
{
    drive = amount;
}

void Distortion::SetTone(float amount)
{
    tone = amount;

    float shaped = tone * tone;

    float cutoff = 1800.0f + shaped * 6200.0f;

    tone_lpf.SetCutoff(cutoff);
}