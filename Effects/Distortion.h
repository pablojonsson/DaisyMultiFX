#pragma once
#ifndef DISTORTION_EFFECT
#define DISTORTION_EFFECT

#include <algorithm>
#ifdef __cplusplus

namespace CustomEffects
{
    class Distortion
    {
        public:
            Distortion() {}
            ~Distortion() {}

            // Initializes Distortion effect
            void Init(float sample_rate);

            // Sets the drive amount of the Distortion
            void setDrive(float amount);

            // Sets the final volume of the Distortion
            void setVolume(float amount);
            
            // Processes a given input and returns the distorted output
            float Process(float in);

        private:
            // drive: How much you distort from 30.0-150.0
            // volume: Gain applied at the end from 0.0-1.0
            // sampling_freq: The sampling freq of the input
            // tone_cutoff: cutoff frequency of lowpass filter in Hz 20-20000
            float drive, volume, sampling_freq;
            int tone_cutoff;
    };
}


#endif
#endif