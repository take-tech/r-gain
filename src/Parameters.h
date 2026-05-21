#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace rgain::param
{
    static constexpr auto gainDb = "gainDb";
    static constexpr auto theme = "theme";

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
}
