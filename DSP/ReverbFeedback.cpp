#include "ReverbFeedback.h"

using namespace CustomDSP;

void ReverbFeedback::Init(float sample_rate)
{
    sampling_freq = sample_rate;

    delay_1.Init();
    delay_2.Init();
    delay_3.Init();
    delay_4.Init();
    delay_5.Init();
    delay_6.Init();
    delay_7.Init();
    delay_8.Init();

    delay_1.SetDelay(1427.0f);
    delay_2.SetDelay(1601.0f);
    delay_3.SetDelay(1879.0f);
    delay_4.SetDelay(2111.0f);
    delay_5.SetDelay(2347.0f);
    delay_6.SetDelay(2539.0f);
    delay_7.SetDelay(2753.0f);
    delay_8.SetDelay(2953.0f);

    filter_1 = 0.0f;
    filter_2 = 0.0f;
    filter_3 = 0.0f;
    filter_4 = 0.0f;
    filter_5 = 0.0f;
    filter_6 = 0.0f;
    filter_7 = 0.0f;
    filter_8 = 0.0f;

    lfo_phase_1 = 0.0f;
    lfo_phase_2 = 0.0f;
    lfo_phase_3 = 0.0f;
    lfo_phase_4 = 0.0f;
    lfo_phase_5 = 0.0f;
    lfo_phase_6 = 0.0f;
    lfo_phase_7 = 0.0f;
    lfo_phase_8 = 0.0f;

    depth = 2.0f;
}

void ReverbFeedback::Process(float inL, float inR, float &outL, float &outR)
{

    lfo_phase_1 += (2.0f * M_PI * 0.11f) / sampling_freq;
    lfo_phase_2 += (2.0f * M_PI * 0.13f) / sampling_freq;
    lfo_phase_3 += (2.0f * M_PI * 0.17f) / sampling_freq;
    lfo_phase_4 += (2.0f * M_PI * 0.19f) / sampling_freq;
    lfo_phase_5 += (2.0f * M_PI * 0.23f) / sampling_freq;
    lfo_phase_6 += (2.0f * M_PI * 0.29f) / sampling_freq;
    lfo_phase_7 += (2.0f * M_PI * 0.31f) / sampling_freq;
    lfo_phase_8 += (2.0f * M_PI * 0.37f) / sampling_freq;

    if (lfo_phase_1 >= 2.0f * M_PI)
        lfo_phase_1 -= 2.0f * M_PI;

    if (lfo_phase_2 >= 2.0f * M_PI)
        lfo_phase_2 -= 2.0f * M_PI;

    if (lfo_phase_3 >= 2.0f * M_PI)
        lfo_phase_3 -= 2.0f * M_PI;

    if (lfo_phase_4 >= 2.0f * M_PI)
        lfo_phase_4 -= 2.0f * M_PI;

    if (lfo_phase_5 >= 2.0f * M_PI)
        lfo_phase_5 -= 2.0f * M_PI;

    if (lfo_phase_6 >= 2.0f * M_PI)
        lfo_phase_6 -= 2.0f * M_PI;

    if (lfo_phase_7 >= 2.0f * M_PI)
        lfo_phase_7 -= 2.0f * M_PI;

    if (lfo_phase_8 >= 2.0f * M_PI)
        lfo_phase_8 -= 2.0f * M_PI;

    float mod1 = sinf(lfo_phase_1) * depth;
    float mod2 = sinf(lfo_phase_2) * depth;
    float mod3 = sinf(lfo_phase_3) * depth;
    float mod4 = sinf(lfo_phase_4) * depth;
    float mod5 = sinf(lfo_phase_5) * depth;
    float mod6 = sinf(lfo_phase_6) * depth;
    float mod7 = sinf(lfo_phase_7) * depth;
    float mod8 = sinf(lfo_phase_8) * depth;

    delay_1.SetDelay(1427.0f + mod1);
    delay_2.SetDelay(1601.0f + mod2);
    delay_3.SetDelay(1879.0f + mod3);
    delay_4.SetDelay(2111.0f + mod4);
    delay_5.SetDelay(2347.0f + mod5);
    delay_6.SetDelay(2539.0f + mod6);
    delay_7.SetDelay(2753.0f + mod7);
    delay_8.SetDelay(2953.0f + mod8);

    float d1 = delay_1.Read();
    float d2 = delay_2.Read();
    float d3 = delay_3.Read();
    float d4 = delay_4.Read();
    float d5 = delay_5.Read();
    float d6 = delay_6.Read();
    float d7 = delay_7.Read();
    float d8 = delay_8.Read();

    const float norm = 0.353553f;

    float fb1 = (d1 + d2 + d3 + d4 + d5 + d6 + d7 + d8) * norm;

    float fb2 = (d1 - d2 + d3 - d4 + d5 - d6 + d7 - d8) * norm;

    float fb3 = (d1 + d2 - d3 - d4 + d5 + d6 - d7 - d8) * norm;

    float fb4 = (d1 - d2 - d3 + d4 + d5 - d6 - d7 + d8) * norm;

    float fb5 = (d1 + d2 + d3 + d4 - d5 - d6 - d7 - d8) * norm;

    float fb6 = (d1 - d2 + d3 - d4 - d5 + d6 - d7 + d8) * norm;

    float fb7 = (d1 + d2 - d3 - d4 - d5 - d6 + d7 + d8) * norm;

    float fb8 = (d1 - d2 - d3 + d4 - d5 + d6 + d7 - d8) * norm;

    filter_1 += damping * (fb1 - filter_1);
    filter_2 += damping * (fb2 - filter_2);
    filter_3 += damping * (fb3 - filter_3);
    filter_4 += damping * (fb4 - filter_4);
    filter_5 += damping * (fb5 - filter_5);
    filter_6 += damping * (fb6 - filter_6);
    filter_7 += damping * (fb7 - filter_7);
    filter_8 += damping * (fb8 - filter_8);

    delay_1.Write(filter_1 * feedback + inL);
    delay_2.Write(filter_2 * feedback + inR);
    delay_3.Write(filter_3 * feedback - inL * 0.7f);
    delay_4.Write(filter_4 * feedback + inR * 0.6f);
    delay_5.Write(filter_5 * feedback + inL * 0.4f);
    delay_6.Write(filter_6 * feedback - inR * 0.5f);
    delay_7.Write(filter_7 * feedback + inL * 0.3f);
    delay_8.Write(filter_8 * feedback + inR * 0.8f);

    outL = (d1 + d2 - d3 + d4 - d5 + d6 + d7 - d8) * 0.353553f;

    outR = (-d1 + d2 + d3 - d4 + d5 + d6 - d7 + d8) * 0.353553f;
}

void ReverbFeedback::setFeedback(float amount)
{
    feedback = amount;
}

void ReverbFeedback::setDamping(float amount)
{
    damping = amount;
}