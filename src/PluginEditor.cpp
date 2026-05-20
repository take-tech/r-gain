#include "PluginEditor.h"

RGainAudioProcessorEditor::RGainAudioProcessorEditor(RGainAudioProcessor& processorRef)
    : AudioProcessorEditor(&processorRef),
      audioProcessor(processorRef)
{
    setSize(320, 220);

    gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 84, 24);
    gainSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffff5a1f));
    gainSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff3a3a3a));
    gainSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffffc15a));
    gainSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    gainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff151515));
    gainSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xff2a2a2a));
    addAndMakeVisible(gainSlider);

    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(gainLabel);

    valueLabel.setText("Output", juce::dontSendNotification);
    valueLabel.setJustificationType(juce::Justification::centred);
    valueLabel.setColour(juce::Label::textColourId, juce::Colour(0xffcfcfcf));
    addAndMakeVisible(valueLabel);

    gainAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.getValueTreeState(),
        rgain::param::gainDb,
        gainSlider);

    startTimerHz(30);
}

RGainAudioProcessorEditor::~RGainAudioProcessorEditor()
{
    stopTimer();
}

void RGainAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff080808));

    g.setColour(juce::Colour(0xff1c1c1c));
    g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(10.0f), 8.0f);

    g.setColour(juce::Colour(0xffff5a1f));
    g.setFont(juce::FontOptions(18.0f, juce::Font::bold));
    g.drawText("R-Gain", 18, 12, 120, 24, juce::Justification::centredLeft);

    auto meterArea = getLocalBounds().removeFromRight(92).toFloat();
    meterArea.removeFromTop(96.0f);
    meterArea.removeFromBottom(32.0f);
    meterArea = meterArea.reduced(18.0f, 0.0f);

    const auto meterWidth = 20.0f;
    const auto gap = 12.0f;

    drawMeter(g, meterArea.withWidth(meterWidth), leftMeterLevel, "L");
    drawMeter(g, meterArea.withX(meterArea.getX() + meterWidth + gap).withWidth(meterWidth), rightMeterLevel, "R");
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

void RGainAudioProcessorEditor::drawMeter(juce::Graphics& g, juce::Rectangle<float> bounds, float level, const juce::String& label)
{
    g.setColour(juce::Colour(0xff050505));
    g.fillRoundedRectangle(bounds, 4.0f);

    g.setColour(juce::Colour(0xff303030));
    g.drawRoundedRectangle(bounds, 4.0f, 1.0f);

    const auto clampedLevel = juce::jlimit(0.0f, 1.0f, level);
    const auto levelDb = juce::Decibels::gainToDecibels(clampedLevel, -60.0f);
    const auto normalized = juce::jmap(juce::jlimit(-60.0f, 0.0f, levelDb), -60.0f, 0.0f, 0.0f, 1.0f);
    const auto fillHeight = bounds.getHeight() * normalized;

    auto fillBounds = bounds.withY(bounds.getBottom() - fillHeight).withHeight(fillHeight).reduced(3.0f);

    g.setGradientFill(juce::ColourGradient(
        juce::Colour(0xffffd15c),
        fillBounds.getCentreX(),
        fillBounds.getY(),
        juce::Colour(0xffff3b1f),
        fillBounds.getCentreX(),
        fillBounds.getBottom(),
        false));
    g.fillRoundedRectangle(fillBounds, 3.0f);

    g.setColour(juce::Colour(0xffd0d0d0));
    g.setFont(juce::FontOptions(12.0f));
    g.drawText(label, bounds.withY(bounds.getBottom() + 4.0f).withHeight(16.0f), juce::Justification::centred);
}
