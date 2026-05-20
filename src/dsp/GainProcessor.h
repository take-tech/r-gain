#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

namespace rgain
{
namespace dsp
{
    class GainProcessor
    {
    public:
        void prepare(double sampleRate, int samplesPerBlock, int numChannels);
        void reset();
        void setGainDb(float newGainDb);
        void processBlock(juce::AudioBuffer<float>& buffer);

    private:
        static float dbToLinear(float gainDb);

        juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> gainLinear { 1.0f };
        double currentSampleRate { 44100.0 };
        int currentNumChannels { 0 };
    };
}
}
