#include "AllPass.h"

using namespace CustomDSP;

void AllPass::Init(float delay_samples, float feedback_amount)
{
    delay.Init();
    delay.SetDelay(delay_samples);

    feedback = feedback_amount;
}

float AllPass::Process(float in)
{
    float delayed = delay.Read();

    float out = delayed - feedback * in;

    float delay_input = in + feedback * out;

    delay.Write(delay_input);

    return out;
}