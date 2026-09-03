#include "FreqCrossover.h"

using namespace CustomDSP;

void FreqCrossover::Init(float sample_rate, float crossover_freq)
{
    sampling_freq = sample_rate;
    constexpr float BUTTERWORTH_Q = 0.70710678f;
    lowL1.SetLowPass(sample_rate, crossover_freq, BUTTERWORTH_Q);
    lowL2.SetLowPass(sample_rate, crossover_freq, BUTTERWORTH_Q);
    highL1.SetHighPass(sample_rate, crossover_freq, BUTTERWORTH_Q);
    highL2.SetHighPass(sample_rate, crossover_freq, BUTTERWORTH_Q);

    lowR1.SetLowPass(sample_rate, crossover_freq, BUTTERWORTH_Q);
    lowR2.SetLowPass(sample_rate, crossover_freq, BUTTERWORTH_Q);
    highR1.SetHighPass(sample_rate, crossover_freq, BUTTERWORTH_Q);
    highR2.SetHighPass(sample_rate, crossover_freq, BUTTERWORTH_Q);
    crossover_frequency = crossover_freq;
}

void FreqCrossover::Process(float inL, float inR, float &low_outL, float &high_outL, float &low_outR, float &high_outR)
{
    low_outL = lowL2.Process(lowL1.Process(inL));
    high_outL = highL2.Process(highL1.Process(inL));

    low_outR = lowR2.Process(lowR1.Process(inR));
    high_outR = highR2.Process(highR1.Process(inR));
}

void FreqCrossover::SetCrossoverFreq(float freq)
{
    crossover_frequency = freq;

    constexpr float Q = 0.70710678f;

    lowL1.SetLowPass(sampling_freq, freq, Q);
    lowL2.SetLowPass(sampling_freq, freq, Q);

    highL1.SetHighPass(sampling_freq, freq, Q);
    highL2.SetHighPass(sampling_freq, freq, Q);

    lowR1.SetLowPass(sampling_freq, freq, Q);
    lowR2.SetLowPass(sampling_freq, freq, Q);

    highR1.SetHighPass(sampling_freq, freq, Q);
    highR2.SetHighPass(sampling_freq, freq, Q);
}