#include "Phaser.h"

using namespace CustomEffects;

struct AllPassStage
{
    float previous_input = 0.0f;
    float previous_output = 0.0f;

    float Process(float in, float a)
    {
        float out = -a * in + previous_input + a * previous_output;

        previous_input = in;
        previous_output = out;

        return out;
    }

    void Reset()
    {
        previous_input = 0.0f;
        previous_output = 0.0f;
    }
};

void Phaser::Init(float sample_rate)
{
}

void Phaser::Process(float inL, float inR, float &outL, float &outR)
{
}