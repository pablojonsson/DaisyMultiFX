#include "Distortion.h"

using namespace CustomEffects;

void Distortion::Init(float sample_rate)
{
    sampling_freq = sample_rate;
    svf.Init(sampling_freq);
    drive = 0.5f;
    tone = 0.5f;
}

float Distortion::Process(float in)
{
    svf.SetMode(CustomDSP::FilterMode::HighPass);
    svf.SetCutoff(250.0f);
    float x;
    svf.Process(in, in, x, x);
    x *= 3.0f * drive;
    if (x > 1.0f)
        x = 1.0f;
    else if (x < -1.0f)
        x = -1.0f;

    float clip1 = x - (x * x * x) / 3.0f;

    float clip2 = clip1 * 5.0f;
    if (clip2 > 0.7f) clip2 = 0.7f;
    else if (clip2 < -0.7f) clip2 = -0.7f;

    float lpf_out;
    svf.SetMode(CustomDSP::FilterMode::LowPass);
    svf.SetCutoff(1000.0f);
    svf.Process(clip2, clip2, lpf_out, lpf_out);
    float hpf_out;
    svf.SetMode(CustomDSP::FilterMode::HighPass);
    svf.SetCutoff(1000.0f);
    svf.Process(clip2, clip2, hpf_out, hpf_out);
    float toned = (1.0f - tone) * lpf_out + tone * hpf_out;

    return toned * dynamic_gain * 1.2f;
}

void Distortion::SetDrive(float amount)
{
    drive = amount;
    dynamic_gain = 1.0f / sqrtf(drive); 
}

void Distortion::SetTone(float amount)
{
    tone = amount;
}