#include "Chorus.h"
#include <cmath>

using namespace CustomEffects;

constexpr float TWO_PI_F = 6.28318530717958647692f;

void Chorus::Init(float sample_rate)
{
    sampling_freq = sample_rate;
    rate = 1.0f;
    depth = 3.0f;
    base_delay = 10.0f;
    mix = .7f;
    left_delay_buffer.Init();
    left_delay_buffer.SetDelay(base_delay);
    right_delay_buffer.Init();
    right_delay_buffer.SetDelay(base_delay);
    phase_increment = (TWO_PI_F * rate) / sampling_freq;
    lfo_sin = 0.0f;
    lfo_cos = 1.0f;

    sin_increment = sinf(phase_increment);
    cos_increment = cosf(phase_increment);
}

__attribute__((noinline)) void Chorus::Process(float inL, float inR, float &outL, float &outR)
{
    float samples_per_ms = sampling_freq * 0.001f;
    float left_delay_samples = (base_delay + lfo_sin * depth) * samples_per_ms;

    left_delay_buffer.SetDelay(left_delay_samples);

    float left_delayed_sample = left_delay_buffer.Read();
    left_delay_buffer.Write(inL);

    outL = mix * left_delayed_sample + (1.0f - mix) * inL;

    float right_delay_samples = (base_delay + lfo_cos * depth) * samples_per_ms;

    right_delay_buffer.SetDelay(right_delay_samples);

    float right_delayed_sample = right_delay_buffer.Read();
    right_delay_buffer.Write(inR);

    outR = mix * right_delayed_sample + (1.0f - mix) * inR;

    float new_sin = lfo_sin * cos_increment + lfo_cos * sin_increment;

    float new_cos = lfo_cos * cos_increment - lfo_sin * sin_increment;

    lfo_sin = new_sin;
    lfo_cos = new_cos;
}

void Chorus::setDepth(float amount)
{
    depth = amount;
}

void Chorus::setRate(float freq)
{
    if (fabsf(freq - rate) > 0.001f)
        phase_increment = (TWO_PI_F * freq) / sampling_freq;
    rate = freq;
}