#include "MasterDiffuser.h"

using namespace CustomDSP;

void MasterDiffuser::Init(float sample_rate)
{
    sampling_freq = sample_rate;
    diff4.Init(sampling_freq);
    diff8.Init(sampling_freq);
    diff16.Init(sampling_freq);
}

void MasterDiffuser::Process(float inL, float inR, float &outL, float &outR)
{
    float l1, r1;
    float l2, r2;

    diff4.Process(inL, inR, l1, r1);
    diff8.Process(l1, r1, l2, r2);
    diff16.Process(l2, r2, outL, outR);
}

void MasterDiffuser::SoftReset()
{
    diff4.SoftReset();
    diff8.SoftReset();
    diff16.SoftReset();
}

void MasterDiffuser::ClearStep()
{
    diff4.ClearStep();
    diff8.ClearStep();
    diff16.ClearStep();
}

