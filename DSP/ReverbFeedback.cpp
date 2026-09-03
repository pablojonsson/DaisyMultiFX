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

    depth = 2.0f;

    const float rates[8] = {0.11f, 0.13f, 0.17f, 0.19f, 0.23f, 0.29f, 0.31f, 0.37f};

    for (int i = 0; i < 8; i++)
    {
        lfo_phase[i] = 0.0f;
        filter[i] = 0.0f;
        phase_increment[i] = TWO_PI_F * rates[i] / sampling_freq;
    }
}

void ReverbFeedback::Process(float inL, float inR, float &outL, float &outR)
{
    if(mod_counter == 0)
    {
        for(int i = 0; i < 8; i++)
        {
            lfo_phase[i] += phase_increment[i] * MOD_UPDATE_DIV;

            if(lfo_phase[i] >= TWO_PI_F)
                lfo_phase[i] -= TWO_PI_F;
        }

        float mod1 = sinf(lfo_phase[0]) * depth;
        float mod2 = sinf(lfo_phase[1]) * depth;
        float mod3 = sinf(lfo_phase[2]) * depth;
        float mod4 = sinf(lfo_phase[3]) * depth;
        float mod5 = sinf(lfo_phase[4]) * depth;
        float mod6 = sinf(lfo_phase[5]) * depth;
        float mod7 = sinf(lfo_phase[6]) * depth;
        float mod8 = sinf(lfo_phase[7]) * depth;

        delay_1.SetDelay(1427.0f + mod1);
        delay_2.SetDelay(1601.0f + mod2);
        delay_3.SetDelay(1879.0f + mod3);
        delay_4.SetDelay(2111.0f + mod4);
        delay_5.SetDelay(2347.0f + mod5);
        delay_6.SetDelay(2539.0f + mod6);
        delay_7.SetDelay(2753.0f + mod7);
        delay_8.SetDelay(2953.0f + mod8);
    }

    mod_counter++;

    if(mod_counter >= MOD_UPDATE_DIV)
        mod_counter = 0;

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

    for(int i = 0; i < 8; i++)
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

void ReverbFeedback::SoftReset()
{
    for(int i = 0; i < 8; i++)
    {
        lfo_phase[i] = 0.0f;
        filter[i] = 0.0f;
    }

    mod_counter = 0;

    clearing = true;
    clear_samples_remaining = 3000;
}

void ReverbFeedback::ClearStep()
{
    if(!clearing)
        return;

    constexpr int CLEAR_PER_CALL = 8;

    int amount = CLEAR_PER_CALL;

    if(clear_samples_remaining < amount)
        amount = clear_samples_remaining;

    for(int i = 0; i < amount; i++)
    {
        delay_1.Write(0.0f);
        delay_2.Write(0.0f);
        delay_3.Write(0.0f);
        delay_4.Write(0.0f);
        delay_5.Write(0.0f);
        delay_6.Write(0.0f);
        delay_7.Write(0.0f);
        delay_8.Write(0.0f);
    }

    clear_samples_remaining -= amount;

    if(clear_samples_remaining <= 0)
        clearing = false;
}


void ReverbFeedback::HardReset()
{
    delay_1.Reset();
    delay_2.Reset();
    delay_3.Reset();
    delay_4.Reset();
    delay_5.Reset();
    delay_6.Reset();
    delay_7.Reset();
    delay_8.Reset();

    SoftReset();
}