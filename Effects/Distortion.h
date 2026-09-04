#pragma once
#ifndef DISTORTION_EFFECT
#define DISTORTION_EFFECT

#include "../DSP/StateVariableFilter.h"
#ifdef __cplusplus

namespace CustomEffects {
class Distortion {
  public:
    Distortion(){};
    ~Distortion(){};

    void Init(float sample_rate);

    float Process(float in);

    void SetDrive(float amount);

    void SetTone(float amount);

  private:
    float drive;
    float tone;
    float sampling_freq;

    CustomDSP::StateVariableFilter input_hpf;
    CustomDSP::StateVariableFilter tone_lpf;
    CustomDSP::StateVariableFilter tone_hpf;
};
} // namespace CustomEffects
#endif
#endif