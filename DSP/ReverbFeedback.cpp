#include "ReverbFeedback.h"

using namespace CustomDSP;

constexpr float TWO_PI_F = 6.28318530717958647692f;

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

    filter[0] = 0.0f;
    filter[1] = 0.0f;
    filter[2] = 0.0f;
    filter[3] = 0.0f;
    filter[4] = 0.0f;
    filter[5] = 0.0f;
    filter[6] = 0.0f;
    filter[7] = 0.0f;

    lfo_phase[0] = 0.0f;
    lfo_phase[1] = 0.0f;
    lfo_phase[2] = 0.0f;
    lfo_phase[3] = 0.0f;
    lfo_phase[4] = 0.0f;
    lfo_phase[5] = 0.0f;
    lfo_phase[6] = 0.0f;
    lfo_phase[7] = 0.0f;

    depth = 2.0f;

    const float rates[8] = {0.11f, 0.13f, 0.17f, 0.19f, 0.23f, 0.29f, 0.31f, 0.37f};

    for (int i = 0; i < 8; i++)
    {
        lfo_phase[i] = 0.0f;
        phase_increment[i] = TWO_PI_F * rates[i] / sampling_freq;
    }
}

__attribute__((noinline)) void ReverbFeedback::Process(float inL, float inR, float &outL,
                                                       float &outR)
{

    float mod[8];

    for (int i = 0; i < 8; i++)
    {
        lfo_phase[i] += phase_increment[i];

        if (lfo_phase[i] >= TWO_PI_F)
        {
            lfo_phase[i] -= TWO_PI_F;
        }

        mod[i] = sinf(lfo_phase[i]) * depth;
    }

    delay_1.SetDelay(1427.0f + mod[0]);
    delay_2.SetDelay(1601.0f + mod[1]);
    delay_3.SetDelay(1879.0f + mod[2]);
    delay_4.SetDelay(2111.0f + mod[3]);
    delay_5.SetDelay(2347.0f + mod[4]);
    delay_6.SetDelay(2539.0f + mod[5]);
    delay_7.SetDelay(2753.0f + mod[6]);
    delay_8.SetDelay(2953.0f + mod[7]);

    float d1 = delay_1.Read();
    float d2 = delay_2.Read();
    float d3 = delay_3.Read();
    float d4 = delay_4.Read();
    float d5 = delay_5.Read();
    float d6 = delay_6.Read();
    float d7 = delay_7.Read();
    float d8 = delay_8.Read();

    float fb[8] = {d1, d2, d3, d4, d5, d6, d7, d8};

    DiffuserMath::Hadamard(fb);

    for (int i = 0; i < 8; i++)
    {
        fb[i] *= 0.353553f;
    }

    for (int i = 0; i < 8; i++)
    {
        filter[i] += damping * (fb[i] - filter[i]);
    }

    delay_1.Write(filter[0] * feedback + inL);
    delay_2.Write(filter[1] * feedback + inR);
    delay_3.Write(filter[2] * feedback - inL * 0.7f);
    delay_4.Write(filter[3] * feedback + inR * 0.6f);
    delay_5.Write(filter[4] * feedback + inL * 0.4f);
    delay_6.Write(filter[5] * feedback - inR * 0.5f);
    delay_7.Write(filter[6] * feedback + inL * 0.3f);
    delay_8.Write(filter[7] * feedback + inR * 0.8f);

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