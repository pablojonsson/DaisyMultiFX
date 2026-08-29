#include "StateVariableFilter.h"
#include <cmath>

using namespace CustomDSP;

void StateVariableFilter::Init(float sample_rate)
{
    sampling_rate = sample_rate;
}

SVFOutput StateVariableFilter::ProcessStage(SVFStage &stage, float inL, float inR)
{
    float low_pass_output_L, band_pass_output_L, high_pass_output_L;
    float low_pass_output_R, band_pass_output_R, high_pass_output_R;

    float v3_L = inL - stage.state2L;
    float v1_L = a1 * stage.state1L + a2 * v3_L;
    float v2_L = stage.state2L + a2 * stage.state1L + a3 * v3_L;

    float v3_R = inR - stage.state2R;
    float v1_R = a1 * stage.state1R + a2 * v3_R;
    float v2_R = stage.state2R + a2 * stage.state1R + a3 * v3_R;

    low_pass_output_L = v2_L;
    band_pass_output_L = v1_L;
    high_pass_output_L = inL - k * v1_L - v2_L;

    low_pass_output_R = v2_R;
    band_pass_output_R = v1_R;
    high_pass_output_R = inR - k * v1_R - v2_R;

    stage.state1L = 2 * v1_L - stage.state1L;
    stage.state2L = 2 * v2_L - stage.state2L;

    stage.state1R = 2 * v1_R - stage.state1R;
    stage.state2R = 2 * v2_R - stage.state2R;

    SVFOutput output;
    output.lowL = low_pass_output_L;
    output.lowR = low_pass_output_R;
    output.bandL = band_pass_output_L;
    output.bandR = band_pass_output_R;
    output.highL = high_pass_output_L;
    output.highR = high_pass_output_R;
    return output;
}

void StateVariableFilter::Process(float inL, float inR, float &outL, float &outR)
{
    SVFOutput output_2_pole = ProcessStage(stage1, inL, inR);
    float selected_output_L, selected_output_R;
    switch (mode)
    {
    case FilterMode::HighPass:
        selected_output_L = output_2_pole.highL;
        selected_output_R = output_2_pole.highR;
        break;
    case FilterMode::BandPass:
        selected_output_L = output_2_pole.bandL;
        selected_output_R = output_2_pole.bandR;
        break;
    default:
        selected_output_L = output_2_pole.lowL;
        selected_output_R = output_2_pole.lowR;
        break;
    }

    if (poles == 4)
    {
        SVFOutput output_4_pole = ProcessStage(stage2, selected_output_L, selected_output_R);
        switch (mode)
        {
        case FilterMode::HighPass:
            selected_output_L = output_4_pole.highL;
            selected_output_R = output_4_pole.highR;
            break;
        case FilterMode::BandPass:
            selected_output_L = output_4_pole.bandL;
            selected_output_R = output_4_pole.bandR;
            break;
        default:
            selected_output_L = output_4_pole.lowL;
            selected_output_R = output_4_pole.lowR;
            break;
        }
    }
    outL = selected_output_L;
    outR = selected_output_R;
}

void StateVariableFilter::SetCutoff(float cutoff)
{
    g = tanf(PI_F * (cutoff / sampling_rate));
    UpdateCoefficients();
}

void StateVariableFilter::SetResonance(float resonance)
{
    k = 1 / resonance;
    UpdateCoefficients();
}

void StateVariableFilter::SetMode(FilterMode new_mode)
{
    mode = new_mode;
}
void StateVariableFilter::SetPoles(int pole_count)
{
    poles = pole_count;
}

void StateVariableFilter::UpdateCoefficients()
{
    a1 = 1 / (1 + g * (g + k));
    a2 = g * a1;
    a3 = g * a2;
}

void StateVariableFilter::Reset()
{
    stage1.Reset();
    stage2.Reset();
}