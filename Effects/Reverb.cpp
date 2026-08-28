#include "Reverb.h"

using namespace CustomEffects;

void Reverb::Init(float sample_rate)
{
    sampling_freq = sample_rate;
    mix = .7f;
    damping = .2f;
    feedback = .7f;

    delay_1.Init();
    delay_2.Init();
    delay_3.Init();
    delay_4.Init();
    pre_delay.Init();

    delay_1.SetDelay(1399.0f);
    delay_2.SetDelay(1801.0f);
    delay_3.SetDelay(2297.0f);
    delay_4.SetDelay(2903.0f);
    pre_delay.SetDelay(720.0f);

    filter_state_1 = 0.0f;
    filter_state_2 = 0.0f;
    filter_state_3 = 0.0f;
    filter_state_4 = 0.0f;

    allpass_1.Init(149.0f, 0.5f);
    allpass_2.Init(263.0f, 0.5f);

    lfo_phase = 0.0f;
    lfo_rate = 0.2f;
    lfo_depth = 5.0f;

    lfo_increment = 2.0f * M_PI * lfo_rate / sampling_freq;
}

float Reverb::Process(float in)
{
    float delayed_input = pre_delay.Read();
    pre_delay.Write(in);

    float diffused = allpass_1.Process(delayed_input);
    diffused = allpass_2.Process(diffused);

    float lfo1 = sinf(lfo_phase);
    float lfo2 = cosf(lfo_phase);

    delay_1.SetDelay(1399.0f + lfo1 * lfo_depth);
    delay_2.SetDelay(1801.0f + lfo2 * lfo_depth);
    delay_3.SetDelay(2297.0f - lfo1 * lfo_depth);
    delay_4.SetDelay(2903.0f - lfo2 * lfo_depth);

    float d1 = delay_1.Read();
    float d2 = delay_2.Read();
    float d3 = delay_3.Read();
    float d4 = delay_4.Read();

    filter_state_1 += damping * (d1 - filter_state_1);
    filter_state_2 += damping * (d2 - filter_state_2);
    filter_state_3 += damping * (d3 - filter_state_3);
    filter_state_4 += damping * (d4 - filter_state_4);

    float fb1 = filter_state_1 + filter_state_2 - filter_state_3 - filter_state_4;

    float fb2 = filter_state_1 - filter_state_2 + filter_state_3 - filter_state_4;

    float fb3 = filter_state_1 - filter_state_2 - filter_state_3 + filter_state_4;

    float fb4 = filter_state_1 + filter_state_2 + filter_state_3 + filter_state_4;

    fb1 *= 0.5f;
    fb2 *= 0.5f;
    fb3 *= 0.5f;
    fb4 *= 0.5f;

    delay_1.Write(diffused + fb1 * feedback);
    delay_2.Write(diffused + fb2 * feedback);
    delay_3.Write(diffused + fb3 * feedback);
    delay_4.Write(diffused + fb4 * feedback);

    float wet = (d1 + d2 + d3 + d4) * 0.4f;

    lfo_phase += lfo_increment;

    if (lfo_phase >= 2.0f * M_PI)
    {
        lfo_phase -= 2.0f * M_PI;
    }

    return in + wet * mix;
}

void Reverb::setDamping(float amount)
{
    damping = amount;
}

void Reverb::setFeedback(float amount)
{
    feedback = amount;
}

void Reverb::setMix(float amount)
{
    mix = amount;
}