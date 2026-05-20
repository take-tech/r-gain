#include "Parameters.h"

namespace rgain::param
{
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

        parameters.push_back(std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID { gainDb, 1 },
            "Gain",
            juce::NormalisableRange<float> { -60.0f, 24.0f, 0.01f },
            0.0f,
            juce::AudioParameterFloatAttributes().withLabel("dB")));

        return { parameters.begin(), parameters.end() };
    }
}
