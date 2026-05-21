#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include <ranze_plugin_ui/Theme.h>

#include "PluginProcessor.h"

class RGainAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                        private juce::Timer
{
public:
    explicit RGainAudioProcessorEditor(RGainAudioProcessor&);
    ~RGainAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    void timerCallback() override;
    void applyTheme();

    RGainAudioProcessor& audioProcessor;
    ranze::ui::Theme theme { ranze::ui::getDefaultTheme() };

    juce::Slider gainSlider;
    juce::Label gainLabel;
    juce::Label valueLabel;
    std::unique_ptr<SliderAttachment> gainAttachment;

    float leftMeterLevel { 0.0f };
    float rightMeterLevel { 0.0f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RGainAudioProcessorEditor)
};
