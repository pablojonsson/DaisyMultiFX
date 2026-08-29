#pragma once

namespace CustomDSP
{
struct DiffuserMath
{
    __attribute__((noinline)) static void Process(float inL, float inR, float feedback,
                                                  const float delayed[8], float writes[8],
                                                  float &outL, float &outR);

    static void Hadamard(float x[8]);
};
} // namespace CustomDSP