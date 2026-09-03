#include "Phaser.h"
#include <cmath>

using namespace CustomEffects;

void Phaser::Init(float sample_rate)
{
    sampling_freq = sample_rate;

    rate     = 0.5f;
    depth    = 0.7f;
    feedback = 0.4f;
    mix      = 0.5f;

    lfo_phase = 0.0f;

    phase_increment = 2 * PI_F * rate / sampling_freq;

    feedback = -0.2f;
    mix = 0.5f;

    Reset();
}

float Phaser::FrequencyToCoefficient(float frequency)
{
    if(frequency < 20.0f)
        frequency = 20.0f;

    if(frequency > sampling_freq * 0.45f)
        frequency = sampling_freq * 0.45f;

    float t = tanf(3.14159265359f * frequency / sampling_freq);

    return (1.0f - t) / (1.0f + t);
}

void Phaser::Process(float inL, float inR, float &outL, float &outR)
{
    if(control_counter == 0)
    {
        float lfoL = sinf(lfo_phase);
        float lfoR = cosf(lfo_phase);

        lfoL = 0.5f * (lfoL + 1.0f);
        lfoR = 0.5f * (lfoR + 1.0f);

        const float minFreq = 200.0f;
        const float maxFreq = 1800.0f;

        float sweepRange = (maxFreq - minFreq) * depth;

        float frequencyL = minFreq + lfoL * sweepRange;
        float frequencyR = minFreq + lfoR * sweepRange;

        aL1 = FrequencyToCoefficient(frequencyL * 0.6f);
        aL2 = FrequencyToCoefficient(frequencyL * 0.8f);
        aL3 = FrequencyToCoefficient(frequencyL);
        aL4 = FrequencyToCoefficient(frequencyL * 1.25f);

        aR1 = FrequencyToCoefficient(frequencyR * 0.6f);
        aR2 = FrequencyToCoefficient(frequencyR * 0.8f);
        aR3 = FrequencyToCoefficient(frequencyR);
        aR4 = FrequencyToCoefficient(frequencyR * 1.25f);
    }

    control_counter++;

    if(control_counter >= CONTROL_DIV)
        control_counter = 0;

    float wetL = inL + feedbackL * feedback;
    float wetR = inR + feedbackR * feedback;

    wetL = stagesL[0].Process(wetL, aL1);
    wetL = stagesL[1].Process(wetL, aL2);
    wetL = stagesL[2].Process(wetL, aL3);
    wetL = stagesL[3].Process(wetL, aL4);

    wetR = stagesR[0].Process(wetR, aR1);
    wetR = stagesR[1].Process(wetR, aR2);
    wetR = stagesR[2].Process(wetR, aR3);
    wetR = stagesR[3].Process(wetR, aR4);

    feedbackL = wetL;
    feedbackR = wetR;

    outL = inL + wetL * mix;
    outR = inR + wetR * mix;

    lfo_phase += phase_increment;

    if(lfo_phase >= 2.0f * PI_F)
        lfo_phase -= 2.0f * PI_F;
}

void Phaser::SetRate(float new_rate)
{
    rate = new_rate;

    phase_increment = 2 * PI_F * rate / sampling_freq;
}

void Phaser::SetDepth(float new_depth)
{
    if(new_depth < 0.0f)
        new_depth = 0.0f;
    else if(new_depth > 1.0f)
        new_depth = 1.0f;

    depth = new_depth;
}

void Phaser::SetFeedback(float new_feedback)
{
    if(new_feedback < -0.95f)
        new_feedback = -0.95f;
    else if(new_feedback > 0.95f)
        new_feedback = 0.95f;

    feedback = new_feedback;
}

void Phaser::SetMix(float new_mix)
{
    if(new_mix < 0.0f)
        new_mix = 0.0f;
    else if(new_mix > 1.0f)
        new_mix = 1.0f;

    mix = new_mix;
}

void Phaser::Reset()
{
    feedbackL = 0.0f;
    feedbackR = 0.0f;

    lfo_phase = 0.0f;
    control_counter = 0;

    for(int i = 0; i < 4; i++)
    {
        stagesL[i].Reset();
        stagesR[i].Reset();
    }
}