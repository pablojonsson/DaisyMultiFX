#include "Diffuser2.h"

using namespace CustomDSP;
void Diffuser2::Init(float sample_rate)
{
    sampling_freq = sample_rate;
    delay_1.Init();
    delay_2.Init();
    delay_3.Init();
    delay_4.Init();
    delay_5.Init();
    delay_6.Init();
    delay_7.Init();
    delay_8.Init();
    delay_1.SetDelay(31.0f);
    delay_2.SetDelay(43.0f);
    delay_3.SetDelay(53.0f);
    delay_4.SetDelay(61.0f);
    delay_5.SetDelay(71.0f);
    delay_6.SetDelay(79.0f);
    delay_7.SetDelay(89.0f);
    delay_8.SetDelay(97.0f);
}

void Diffuser2::Process(float inL, float inR, float &outL, float &outR)
{
    float d[8];

    d[0] = delay_1.Read();
    d[1] = delay_2.Read();
    d[2] = delay_3.Read();
    d[3] = delay_4.Read();
    d[4] = delay_5.Read();
    d[5] = delay_6.Read();
    d[6] = delay_7.Read();
    d[7] = delay_8.Read();

    float write[8];

    DiffuserMath::Process(inL, inR, feedback, d, write, outL, outR);

    delay_1.Write(write[0]);
    delay_2.Write(write[1]);
    delay_3.Write(write[2]);
    delay_4.Write(write[3]);
    delay_5.Write(write[4]);
    delay_6.Write(write[5]);
    delay_7.Write(write[6]);
    delay_8.Write(write[7]);
}

void Diffuser2::Reset()
{
    delay_1.Reset();
    delay_2.Reset();
    delay_3.Reset();
    delay_4.Reset();
    delay_5.Reset();
    delay_6.Reset();
    delay_7.Reset();
    delay_8.Reset();
}