#pragma once
#ifndef SVF_DSP
#define SVF_DSP

#include <daisysp.h>

#ifdef __cplusplus
namespace CustomDSP
{

enum class FilterMode
{
    LowPass,
    BandPass,
    HighPass
};

struct SVFOutput
{
    float lowL;
    float bandL;
    float highL;

    float lowR;
    float bandR;
    float highR;
};

struct SVFStage
{
    float state1L = 0.0f;
    float state2L = 0.0f;

    float state1R = 0.0f;
    float state2R = 0.0f;

    void Reset()
    {
        state1L = 0.0f;
        state2L = 0.0f;

        state1R = 0.0f;
        state2R = 0.0f;
    }
};

class StateVariableFilter
{
  public:
    StateVariableFilter() = default;

    void Init(float sample_rate);

    void Process(float inL, float inR, float &outL, float &outR);

    void SetCutoff(float freq);
    void SetResonance(float resonance);

    void SetMode(FilterMode new_mode);
    void SetPoles(int pole_count);

    void Reset();

  private:
    SVFOutput ProcessStage(SVFStage &stage, float inL, float inR);

    void UpdateCoefficients();

    float sampling_rate;

    // SVF coefficients
    float g = 0.0f;
    float k = 1.41421356f;
    float a1 = 0.0f;
    float a2 = 0.0f;
    float a3 = 0.0f;

    int poles = 2;

    FilterMode mode = FilterMode::LowPass;

    SVFStage stage1;
    SVFStage stage2;
};

} // namespace CustomDSP
#endif
#endif