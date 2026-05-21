#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include <ranze_plugin_ui/Theme.h>

#include "PluginProcessor.h"

class RGainAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                        private juce::AudioProcessorValueTreeState::Listener,
                                        private juce::AsyncUpdater,
                                        private juce::Timer
{
public:
    explicit RGainAudioProcessorEditor(RGainAudioProcessor&);
    ~RGainAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    class ThemeIconButton final : public juce::Button
    {
    public:
        ThemeIconButton();
        void setTheme(ranze::ui::Theme newTheme);
        void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    private:
        ranze::ui::Theme theme { ranze::ui::getDefaultTheme() };
    };

    void timerCallback() override;
    void configureThemeButton();
    void showThemeMenu();
    void setThemeIndex(int themeIndex);
    void applyTheme();
    int getCurrentThemeIndex() const;
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    void handleAsyncUpdate() override;

    RGainAudioProcessor& audioProcessor;
    ranze::ui::Theme theme { ranze::ui::getDefaultTheme() };

    juce::Slider gainSlider;
    juce::Label gainLabel;
    juce::Label valueLabel;
    std::unique_ptr<SliderAttachment> gainAttachment;
    ThemeIconButton themeButton;
    juce::ComboBox themeSelector;
    std::unique_ptr<ComboBoxAttachment> themeAttachment;

    float leftMeterLevel { 0.0f };
    float rightMeterLevel { 0.0f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RGainAudioProcessorEditor)
};
