#pragma once
#ifndef BIQUAD_DSP
#define BIQUAD_DSP

#include <cmath>
#include <daisysp.h>
#ifdef __cplusplus

namespace CustomDSP {
class Biquad {
  public:
    void SetLowPass(float sample_rate, float cutoff, float q);
    void SetHighPass(float sample_rate, float cutoff, float q);

    float Process(float in);

    void Reset();

  private:
    float b0 = 0.0f;
    float b1 = 0.0f;
    float b2 = 0.0f;
    float a1 = 0.0f;
    float a2 = 0.0f;

    float z1 = 0.0f;
    float z2 = 0.0f;
};
} // namespace CustomDSP

#endif
#endif