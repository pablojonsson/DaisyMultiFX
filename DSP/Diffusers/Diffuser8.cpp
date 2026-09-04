#include "Diffuser8.h"

using namespace CustomDSP;
void Diffuser8::Init(float sample_rate) {
    sampling_freq = sample_rate;
    delay_1.Init();
    delay_2.Init();
    delay_3.Init();
    delay_4.Init();
    delay_5.Init();
    delay_6.Init();
    delay_7.Init();
    delay_8.Init();
    delay_1.SetDelay(151.0f);
    delay_2.SetDelay(179.0f);
    delay_3.SetDelay(211.0f);
    delay_4.SetDelay(239.0f);
    delay_5.SetDelay(277.0f);
    delay_6.SetDelay(313.0f);
    delay_7.SetDelay(347.0f);
    delay_8.SetDelay(383.0f);
}

void Diffuser8::Process(float inL, float inR, float &outL, float &outR) {
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

void Diffuser8::SoftReset() {
    clearing = true;
    clear_samples_remaining = 383;
}

void Diffuser8::ClearStep() {
    if (!clearing)
        return;

    constexpr int CLEAR_PER_CALL = 8;

    int amount = CLEAR_PER_CALL;

    if (clear_samples_remaining < amount)
        amount = clear_samples_remaining;

    for (int i = 0; i < amount; i++) {
        delay_1.Write(0.0f);
        delay_2.Write(0.0f);
        delay_3.Write(0.0f);
        delay_4.Write(0.0f);
        delay_5.Write(0.0f);
        delay_6.Write(0.0f);
        delay_7.Write(0.0f);
        delay_8.Write(0.0f);
    }

    clear_samples_remaining -= amount;

    if (clear_samples_remaining <= 0)
        clearing = false;
}