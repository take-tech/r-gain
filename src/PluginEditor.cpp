#include "PluginEditor.h"

#include <cmath>

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

    configureThemeButton();
    audioProcessor.getValueTreeState().addParameterListener(rgain::param::theme, this);
    applyTheme();
    startTimerHz(30);
}

RGainAudioProcessorEditor::~RGainAudioProcessorEditor()
{
    stopTimer();
    audioProcessor.getValueTreeState().removeParameterListener(rgain::param::theme, this);
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
    themeButton.setBounds(meterSpace.removeFromTop(28).removeFromRight(28));
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

void RGainAudioProcessorEditor::configureThemeButton()
{
    themeSelector.addItem("Ember", 1);
    themeSelector.addItem("Amber", 2);
    themeSelector.addItem("Ruby", 3);
    themeSelector.addItem("Graphite", 4);
    themeSelector.addItem("Blush", 5);
    themeSelector.onChange = [this] { applyTheme(); };
    addChildComponent(themeSelector);

    themeAttachment = std::make_unique<ComboBoxAttachment>(
        audioProcessor.getValueTreeState(),
        rgain::param::theme,
        themeSelector);

    themeButton.onClick = [this] { showThemeMenu(); };
    addAndMakeVisible(themeButton);
}

void RGainAudioProcessorEditor::showThemeMenu()
{
    juce::PopupMenu menu;
    const auto selectedIndex = getCurrentThemeIndex();

    menu.addItem(1, "Ember", true, selectedIndex == 0);
    menu.addItem(2, "Amber", true, selectedIndex == 1);
    menu.addItem(3, "Ruby", true, selectedIndex == 2);
    menu.addItem(4, "Graphite", true, selectedIndex == 3);
    menu.addItem(5, "Blush", true, selectedIndex == 4);

    menu.showMenuAsync(
        juce::PopupMenu::Options().withTargetComponent(themeButton),
        [this] (int result)
        {
            if (result > 0)
                setThemeIndex(result - 1);
        });
}

void RGainAudioProcessorEditor::setThemeIndex(int themeIndex)
{
    themeSelector.setSelectedId(themeIndex + 1, juce::sendNotificationSync);
}

void RGainAudioProcessorEditor::applyTheme()
{
    const auto themeIndex = getCurrentThemeIndex();
    theme = ranze::ui::getThemeForIndex(themeIndex);

    ranze::ui::applyRotarySliderTheme(gainSlider, theme, ranze::ui::getThemeIdForIndex(themeIndex));
    ranze::ui::applyLabelTheme(gainLabel, theme);
    ranze::ui::applyLabelTheme(valueLabel, theme);
    valueLabel.setColour(juce::Label::textColourId, theme.mutedText);
    themeButton.setTheme(theme);

    repaint();
}

int RGainAudioProcessorEditor::getCurrentThemeIndex() const
{
    const auto selectedId = themeSelector.getSelectedId();

    if (selectedId > 0)
        return juce::jlimit(0, 4, selectedId - 1);

    if (auto* value = audioProcessor.getValueTreeState().getRawParameterValue(rgain::param::theme))
        return juce::jlimit(0, 4, static_cast<int>(std::round(value->load())));

    return 0;
}

void RGainAudioProcessorEditor::parameterChanged(const juce::String& parameterID, float newValue)
{
    juce::ignoreUnused(newValue);

    if (parameterID == rgain::param::theme)
        triggerAsyncUpdate();
}

void RGainAudioProcessorEditor::handleAsyncUpdate()
{
    applyTheme();
}

RGainAudioProcessorEditor::ThemeIconButton::ThemeIconButton()
    : juce::Button("Theme")
{
    setTitle("Theme");
}

void RGainAudioProcessorEditor::ThemeIconButton::setTheme(ranze::ui::Theme newTheme)
{
    theme = newTheme;
    repaint();
}

void RGainAudioProcessorEditor::ThemeIconButton::paintButton(
    juce::Graphics& g,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
    const auto bounds = getLocalBounds().toFloat().reduced(1.0f);
    const auto alpha = shouldDrawButtonAsDown ? 0.95f : (shouldDrawButtonAsHighlighted ? 0.85f : 0.72f);

    g.setColour(theme.control.withAlpha(alpha));
    g.fillRoundedRectangle(bounds, 7.0f);

    g.setColour(theme.accent.withAlpha(0.8f));
    g.drawRoundedRectangle(bounds, 7.0f, 1.2f);

    const auto dotArea = bounds.reduced(7.0f, 6.0f);
    const auto radius = 3.5f;
    g.setColour(theme.accent);
    g.fillEllipse(dotArea.getX(), dotArea.getY() + 4.0f, radius * 2.0f, radius * 2.0f);
    g.setColour(theme.accentAlt);
    g.fillEllipse(dotArea.getCentreX() - radius, dotArea.getY(), radius * 2.0f, radius * 2.0f);
    g.setColour(theme.mutedText);
    g.fillEllipse(dotArea.getRight() - radius * 2.0f, dotArea.getY() + 4.0f, radius * 2.0f, radius * 2.0f);
}
