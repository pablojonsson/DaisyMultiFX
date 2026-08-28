#include "Chorus.h"
#include <cmath>

using namespace CustomEffects;

void Chorus::Init(float sample_rate)
{
    sampling_freq = sample_rate;
    rate = 1.0f;
    depth = 3.0f;
    base_delay = 10.0f;
    lfo_phase = 0.f;
    mix = .7f;
    left_delay_buffer.Init();
    left_delay_buffer.SetDelay(base_delay);
    right_delay_buffer.Init();
    right_delay_buffer.SetDelay(base_delay);
    phase_increment = (2 * M_PI * rate) / sampling_freq;
}

void Chorus::Process(float left_in, float right_in, float &left_out, float &right_out)
{
    float left_lfo_value = sin(lfo_phase);
    float left_current_delay_ms = base_delay + left_lfo_value * depth;
    float left_delay_samples = left_current_delay_ms * sampling_freq / 1000;
    left_delay_buffer.SetDelay(left_delay_samples);
    float left_delayed_sample = left_delay_buffer.Read();
    left_delay_buffer.Write(left_in);
    left_out = (mix * left_delayed_sample) + (1 - mix) * left_in;

    float right_lfo_value = cos(lfo_phase);
    float right_current_delay_ms = base_delay + right_lfo_value * depth;
    float right_delay_samples = right_current_delay_ms * sampling_freq / 1000;
    right_delay_buffer.SetDelay(right_delay_samples);
    float right_delayed_sample = right_delay_buffer.Read();
    right_delay_buffer.Write(right_in);
    right_out = (mix * right_delayed_sample) + (1 - mix) * right_in;

    lfo_phase += phase_increment;
    if (lfo_phase >= 2.0f * M_PI)
    {
        lfo_phase -= 2.0f * M_PI;
    }
}

void Chorus::setDepth(float amount)
{
    depth = amount;
}

void Chorus::setRate(float freq)
{
    if (fabsf(freq - rate) > 0.001f)
        phase_increment = (2 * M_PI * freq) / sampling_freq;
    rate = freq;
}