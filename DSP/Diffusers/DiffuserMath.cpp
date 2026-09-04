#include "DiffuserMath.h"

namespace CustomDSP {
void DiffuserMath::Hadamard(float x[8]) {
    float a0 = x[0] + x[1];
    float a1 = x[0] - x[1];
    float a2 = x[2] + x[3];
    float a3 = x[2] - x[3];
    float a4 = x[4] + x[5];
    float a5 = x[4] - x[5];
    float a6 = x[6] + x[7];
    float a7 = x[6] - x[7];

    float b0 = a0 + a2;
    float b1 = a1 + a3;
    float b2 = a0 - a2;
    float b3 = a1 - a3;
    float b4 = a4 + a6;
    float b5 = a5 + a7;
    float b6 = a4 - a6;
    float b7 = a5 - a7;

    float norm = 0.35355339059f;

    x[0] = (b0 + b4) * norm;
    x[1] = (b1 + b5) * norm;
    x[2] = (b2 + b6) * norm;
    x[3] = (b3 + b7) * norm;
    x[4] = (b0 - b4) * norm;
    x[5] = (b1 - b5) * norm;
    x[6] = (b2 - b6) * norm;
    x[7] = (b3 - b7) * norm;
}

void DiffuserMath::Process(float inL, float inR, float feedback, const float delayed[8],
                           float writes[8], float &outL, float &outR) {
    float s[8];

    for (int i = 0; i < 8; i++) {
        s[i] = delayed[i];
    }

    Hadamard(s);

    writes[0] = s[0] * feedback + inL;
    writes[1] = s[1] * feedback + inR;
    writes[2] = s[2] * feedback - inL * 0.7f;
    writes[3] = s[3] * feedback + inR * 0.6f;
    writes[4] = s[4] * feedback + inL * 0.4f;
    writes[5] = s[5] * feedback - inR * 0.5f;
    writes[6] = s[6] * feedback + inL * 0.3f;
    writes[7] = s[7] * feedback + inR * 0.8f;

    float wetL = (delayed[0] + delayed[1] - delayed[2] + delayed[3] - delayed[4] + delayed[5] +
                  delayed[6] - delayed[7]) *
                 0.353553f;

    float wetR = (-delayed[0] + delayed[1] + delayed[2] - delayed[3] + delayed[4] + delayed[5] -
                  delayed[6] + delayed[7]) *
                 0.353553f;

    outL = inL * 0.7f + wetL * 0.3f;
    outR = inR * 0.7f + wetR * 0.3f;
}
} // namespace CustomDSP