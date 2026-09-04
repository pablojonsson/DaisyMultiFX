#include "AllPass.h"

using namespace CustomDSP;

void AllPass::Init(float sample_rate) {
    last_input = 0.0f;
    last_output = 0.0f;
    sampling_freq = sample_rate;
}

float AllPass::Process(float in) {
    float output = -1 * alpha * in + last_input + alpha * last_output;
    last_output = output;
    last_input = in;
    return output;
}

void AllPass::Reset() {
    last_input = 0.0f;
    last_output = 0.0f;
}

float AllPass::SetAlpha(float frequency) {
    float c = tanf(PI_F * frequency / sampling_freq);
    alpha = (c - 1) / (c + 1);
}