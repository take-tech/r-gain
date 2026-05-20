#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace rgain::param
{
    static constexpr auto gainDb = "gainDb";

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
}
