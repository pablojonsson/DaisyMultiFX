#include "Reverb.h"

using namespace CustomEffects;

void Reverb::Init(float sample_rate)
{
    sampling_freq = sample_rate;
    mix = .7f;
    damping = .2f;
    feedback = .7f;
    diffuser.Init(sampling_freq);
    reverb_feedback.Init(sampling_freq);
    reverb_feedback.setDamping(damping);
    reverb_feedback.setFeedback(feedback);
}

void Reverb::Process(float inL, float inR, float &outL, float &outR)
{
    float diffL, diffR;
    float wetL, wetR;

    diffuser.Process(inL, inR, diffL, diffR);
    reverb_feedback.Process(diffL, diffR, wetL, wetR);

    outL = inL + wetL * mix;
    outR = inR + wetR * mix;
}

void Reverb::setDamping(float amount)
{
    damping = amount;
    reverb_feedback.setDamping(damping);
}

void Reverb::setFeedback(float amount)
{
    feedback = amount;
    reverb_feedback.setFeedback(feedback);
}

void Reverb::setMix(float amount)
{
    mix = amount;
}