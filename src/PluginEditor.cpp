#include "PluginEditor.h"

RGainAudioProcessorEditor::RGainAudioProcessorEditor(RGainAudioProcessor& processorRef)
    : AudioProcessorEditor(&processorRef),
      audioProcessor(processorRef)
{
    setSize(320, 220);

    gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 84, 24);
    addAndMakeVisible(gainSlider);

    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(gainLabel);

    valueLabel.setText("Output", juce::dontSendNotification);
    valueLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(valueLabel);

    gainAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.getValueTreeState(),
        rgain::param::gainDb,
        gainSlider);

    applyTheme();
    startTimerHz(30);
}

RGainAudioProcessorEditor::~RGainAudioProcessorEditor()
{
    stopTimer();
}

void RGainAudioProcessorEditor::paint(juce::Graphics& g)
{
    ranze::ui::drawPluginPanel(g, getLocalBounds(), theme);
    ranze::ui::drawPluginTitle(g, { 18, 12, 120, 24 }, theme, "R-Gain");

    auto meterArea = getLocalBounds().removeFromRight(92).toFloat();
    meterArea.removeFromTop(96.0f);
    meterArea.removeFromBottom(32.0f);
    meterArea = meterArea.reduced(18.0f, 0.0f);

    const auto meterWidth = 20.0f;
    const auto gap = 12.0f;

    ranze::ui::drawVerticalPeakMeter(g, meterArea.withWidth(meterWidth), theme, leftMeterLevel, "L");
    ranze::ui::drawVerticalPeakMeter(g, meterArea.withX(meterArea.getX() + meterWidth + gap).withWidth(meterWidth), theme, rightMeterLevel, "R");
}

void RGainAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(18);
    bounds.removeFromTop(26);

    auto meterSpace = bounds.removeFromRight(86);
    valueLabel.setBounds(meterSpace.removeFromTop(30).withTrimmedTop(6));

    gainLabel.setBounds(bounds.removeFromTop(24));
    gainSlider.setBounds(bounds.reduced(10, 0));
}

void RGainAudioProcessorEditor::timerCallback()
{
    leftMeterLevel = audioProcessor.getLeftMeterLevel();
    rightMeterLevel = audioProcessor.getRightMeterLevel();
    repaint();
}

void RGainAudioProcessorEditor::applyTheme()
{
    theme = ranze::ui::getDefaultTheme();

    ranze::ui::applyRotarySliderTheme(gainSlider, theme);
    ranze::ui::applyLabelTheme(gainLabel, theme);
    ranze::ui::applyLabelTheme(valueLabel, theme);

    repaint();
}
