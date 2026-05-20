#include "GainProcessor.h"

#include <juce_audio_basics/juce_audio_basics.h>

namespace rgain
{
namespace dsp
{
    void GainProcessor::prepare(double sampleRate, int, int numChannels)
    {
        currentSampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
        currentNumChannels = numChannels;

        gainLinear.reset(currentSampleRate, 0.02);
        gainLinear.setCurrentAndTargetValue(1.0f);
    }

    void GainProcessor::reset()
    {
        gainLinear.reset(currentSampleRate, 0.02);
    }

    void GainProcessor::setGainDb(float newGainDb)
    {
        gainLinear.setTargetValue(dbToLinear(newGainDb));
    }

    void GainProcessor::processBlock(juce::AudioBuffer<float>& buffer)
    {
        const auto numChannels = currentNumChannels > 0
            ? juce::jmin(currentNumChannels, buffer.getNumChannels())
            : buffer.getNumChannels();
        const auto numSamples = buffer.getNumSamples();

        if (gainLinear.isSmoothing())
        {
            for (int sample = 0; sample < numSamples; ++sample)
            {
                const auto gain = gainLinear.getNextValue();

                for (int channel = 0; channel < numChannels; ++channel)
                    buffer.setSample(channel, sample, buffer.getSample(channel, sample) * gain);
            }

            return;
        }

        buffer.applyGain(0, numSamples, gainLinear.getCurrentValue());
    }

    float GainProcessor::dbToLinear(float gainDb)
    {
        if (gainDb <= -60.0f)
            return 0.0f;

        return juce::Decibels::decibelsToGain(gainDb);
    }
}
}
