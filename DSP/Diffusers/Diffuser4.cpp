#include "Diffuser4.h"

using namespace CustomDSP;
void Diffuser4::Init(float sample_rate)
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
    delay_1.SetDelay(73.0f);
    delay_2.SetDelay(89.0f);
    delay_3.SetDelay(107.0f);
    delay_4.SetDelay(127.0f);
    delay_5.SetDelay(149.0f);
    delay_6.SetDelay(163.0f);
    delay_7.SetDelay(181.0f);
    delay_8.SetDelay(197.0f);
}

void Diffuser4::Process(float inL, float inR, float &outL, float &outR)
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