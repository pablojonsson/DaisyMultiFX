#include "Biquad.h"
using namespace CustomDSP;

float Biquad::Process(float in)
{
    float out = b0 * in + z1;

    z1 = b1 * in - a1 * out + z2;
    z2 = b2 * in - a2 * out;

    return out;
}

void Biquad::SetLowPass(float sample_rate,
                        float cutoff,
                        float q)
{
    float omega = 2.0f * PI_F * cutoff / sample_rate;

    float cos_w = cosf(omega);
    float sin_w = sinf(omega);

    float alpha = sin_w / (2.0f * q);

    float a0 = 1.0f + alpha;

    b0 = ((1.0f - cos_w) * 0.5f) / a0;
    b1 = (1.0f - cos_w) / a0;
    b2 = ((1.0f - cos_w) * 0.5f) / a0;

    a1 = (-2.0f * cos_w) / a0;
    a2 = (1.0f - alpha) / a0;
}

void Biquad::SetHighPass(float sample_rate,
                         float cutoff,
                         float q)
{
    float omega = 2.0f * PI_F * cutoff / sample_rate;

    float cos_w = cosf(omega);
    float sin_w = sinf(omega);

    float alpha = sin_w / (2.0f * q);

    float a0 = 1.0f + alpha;

    b0 = ((1.0f + cos_w) * 0.5f) / a0;
    b1 = -(1.0f + cos_w) / a0;
    b2 = ((1.0f + cos_w) * 0.5f) / a0;

    a1 = (-2.0f * cos_w) / a0;
    a2 = (1.0f - alpha) / a0;
}