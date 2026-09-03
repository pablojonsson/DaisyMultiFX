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
    dynamic_gain = 1.0f;
}

float Distortion::Process(float in)
{
    float x;
    float dummy;

    input_hpf.Process(in, in, x, dummy);

    float gain = 1.0f + drive * 12.0f;
    x *= gain;

    float distorted = x / (1.0f + fabsf(x));

    float low;
    float high;

    tone_lpf.Process(distorted, distorted, low, dummy);
    tone_hpf.Process(distorted, distorted, high, dummy);

    float toned = low * (1.0f - tone)
                + high * tone;

    return toned * dynamic_gain;
}

void Distortion::SetDrive(float amount)
{
    drive = amount;

    dynamic_gain = 1.0f / (1.0f + drive * 1.5f);
}

void Distortion::SetTone(float amount)
{
    tone = amount;
}