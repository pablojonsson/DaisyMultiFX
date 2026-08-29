#include "MasterDiffuser.h"

using namespace CustomDSP;

void MasterDiffuser::Init(float sample_rate)
{
    sampling_freq = sample_rate;
    diff2.Init(sampling_freq);
    diff4.Init(sampling_freq);
    diff8.Init(sampling_freq);
    diff16.Init(sampling_freq);
}

__attribute__((noinline)) void MasterDiffuser::Process(float inL, float inR, float &outL,
                                                       float &outR)
{
    float l1, r1;
    float l2, r2;
    float l3, r3;

    diff2.Process(inL, inR, l1, r1);
    diff4.Process(l1, r1, l2, r2);
    diff8.Process(l2, r2, l3, r3);
    diff16.Process(l3, r3, outL, outR);
}