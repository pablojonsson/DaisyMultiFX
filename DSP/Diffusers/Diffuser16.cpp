#include "Diffuser16.h"

using namespace CustomDSP;
void Diffuser16::Init(float sample_rate)
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
    delay_1.SetDelay(282.0f);
    delay_2.SetDelay(332.0f);
    delay_3.SetDelay(389.0f);
    delay_4.SetDelay(457.0f);
    delay_5.SetDelay(523.0f);
    delay_6.SetDelay(601.0f);
    delay_7.SetDelay(673.0f);
    delay_8.SetDelay(743.0f);
}

void Diffuser16::Duplicate(float in)
{
    signal_1 = in;
    signal_2 = in;
    signal_3 = in;
    signal_4 = in;
    signal_5 = in;
    signal_6 = in;
    signal_7 = in;
    signal_8 = in;
}

void Diffuser16::Shuffle()
{
    std::swap(signal_1, signal_5);
    std::swap(signal_2, signal_6);
    std::swap(signal_3, signal_7);
    std::swap(signal_4, signal_8);

    signal_2 = -signal_2;
    signal_4 = -signal_4;
    signal_6 = -signal_6;
    signal_8 = -signal_8;
}

void Diffuser16::Hadamard()
{
    float s1 = signal_1;
    float s2 = signal_2;
    float s3 = signal_3;
    float s4 = signal_4;
    float s5 = signal_5;
    float s6 = signal_6;
    float s7 = signal_7;
    float s8 = signal_8;

    signal_1 = (s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8) * 0.353553f;

    signal_2 = (s1 - s2 + s3 - s4 + s5 - s6 + s7 - s8) * 0.353553f;

    signal_3 = (s1 + s2 - s3 - s4 + s5 + s6 - s7 - s8) * 0.353553f;

    signal_4 = (s1 - s2 - s3 + s4 + s5 - s6 - s7 + s8) * 0.353553f;

    signal_5 = (s1 + s2 + s3 + s4 - s5 - s6 - s7 - s8) * 0.353553f;

    signal_6 = (s1 - s2 + s3 - s4 - s5 + s6 - s7 + s8) * 0.353553f;

    signal_7 = (s1 + s2 - s3 - s4 - s5 - s6 + s7 + s8) * 0.353553f;

    signal_8 = (s1 - s2 - s3 + s4 - s5 + s6 + s7 - s8) * 0.353553f;
}

void Diffuser16::Process(float inL, float inR, float &outL, float &outR)
{
    float d1 = delay_1.Read();
    float d2 = delay_2.Read();
    float d3 = delay_3.Read();
    float d4 = delay_4.Read();
    float d5 = delay_5.Read();
    float d6 = delay_6.Read();
    float d7 = delay_7.Read();
    float d8 = delay_8.Read();

    signal_1 = d1;
    signal_2 = d2;
    signal_3 = d3;
    signal_4 = d4;
    signal_5 = d5;
    signal_6 = d6;
    signal_7 = d7;
    signal_8 = d8;

    Hadamard();

    delay_1.Write(signal_1 * feedback + inL);
    delay_2.Write(signal_2 * feedback + inR);
    delay_3.Write(signal_3 * feedback - inL * 0.7f);
    delay_4.Write(signal_4 * feedback + inR * 0.6f);
    delay_5.Write(signal_5 * feedback + inL * 0.4f);
    delay_6.Write(signal_6 * feedback - inR * 0.5f);
    delay_7.Write(signal_7 * feedback + inL * 0.3f);
    delay_8.Write(signal_8 * feedback + inR * 0.8f);

    float wetL = (d1 + d2 - d3 + d4 - d5 + d6 + d7 - d8) * 0.353553f;

    float wetR = (-d1 + d2 + d3 - d4 + d5 + d6 - d7 + d8) * 0.353553f;

    outL = inL * 0.7f + wetL * 0.3f;
    outR = inR * 0.7f + wetR * 0.3f;
}
