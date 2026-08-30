#include "Delay.h"

using namespace CustomEffects;

void Delay::Init(float sample_rate)
{
    sampling_freq = sample_rate;

    delayL.Init();
    delayR.Init();

    current_delay = 4800.0f;
    target_delay = 4800.0f;

    feedback = 0.0f;
    target_feedback = 0.0f;

    delayL.SetDelay(current_delay);
    delayR.SetDelay(current_delay);

    mix = 0.5f;
}

void Delay::Process(float inL, float inR, float &outL, float &outR)
{
    float wetL = delayL.Read();
    float wetR = delayR.Read();

    feedback += 0.001f * (target_feedback - feedback);

    current_delay += 0.0005f * (target_delay - current_delay);

    delayL.SetDelay(current_delay);
    delayR.SetDelay(current_delay);

    filtered_wetL += damping * (wetL - filtered_wetL);
    filtered_wetR += damping * (wetR - filtered_wetR);

    delayL.Write(inL + filtered_wetL * feedback);
    delayR.Write(inR + filtered_wetR * feedback);

    outL = inL * (1.0f - mix) + wetL * mix;
    outR = inR * (1.0f - mix) + wetR * mix;
}

void Delay::SetDelayTime(float time_in_ms)
{
    target_delay = (time_in_ms / 1000.0f) * sampling_freq;
}

void Delay::SetFeedback(float amount)
{
    target_feedback = amount;
}

void Delay::SetMix(float amount)
{
    mix = amount;
}

void Delay::Reset()
{
    delayL.Reset();
    delayR.Reset();
}