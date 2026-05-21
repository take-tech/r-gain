#include "PluginProcessor.h"

#include "PluginEditor.h"
#include "UserSettings.h"

RGainAudioProcessor::RGainAudioProcessor()
    : AudioProcessor(BusesProperties()
          .withInput("Input", juce::AudioChannelSet::stereo(), true)
          .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", rgain::param::createParameterLayout())
{
    if (const auto themeIndex = rgain::settings::loadDefaultThemeIndex())
    {
        if (auto* parameter = apvts.getParameter(rgain::param::theme))
            parameter->setValueNotifyingHost(parameter->convertTo0to1(static_cast<float>(*themeIndex)));
    }
}

void RGainAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    gainProcessor.prepare(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    gainProcessor.setGainDb(*apvts.getRawParameterValue(rgain::param::gainDb));
    leftMeterLevel.store(0.0f, std::memory_order_relaxed);
    rightMeterLevel.store(0.0f, std::memory_order_relaxed);
}

void RGainAudioProcessor::releaseResources()
{
    gainProcessor.reset();
}

bool RGainAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto& mainInput = layouts.getMainInputChannelSet();
    const auto& mainOutput = layouts.getMainOutputChannelSet();

    if (mainInput != mainOutput)
        return false;

    return mainOutput == juce::AudioChannelSet::mono()
        || mainOutput == juce::AudioChannelSet::stereo();
}

void RGainAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    const auto totalInputChannels = getTotalNumInputChannels();
    const auto totalOutputChannels = getTotalNumOutputChannels();

    for (auto channel = totalInputChannels; channel < totalOutputChannels; ++channel)
        buffer.clear(channel, 0, buffer.getNumSamples());

    gainProcessor.setGainDb(*apvts.getRawParameterValue(rgain::param::gainDb));
    gainProcessor.processBlock(buffer);
    updateOutputMeter(buffer);
}

juce::AudioProcessorEditor* RGainAudioProcessor::createEditor()
{
    return new RGainAudioProcessorEditor(*this);
}

bool RGainAudioProcessor::hasEditor() const
{
    return true;
}

const juce::String RGainAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RGainAudioProcessor::acceptsMidi() const
{
    return false;
}

bool RGainAudioProcessor::producesMidi() const
{
    return false;
}

bool RGainAudioProcessor::isMidiEffect() const
{
    return false;
}

double RGainAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RGainAudioProcessor::getNumPrograms()
{
    return 1;
}

int RGainAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RGainAudioProcessor::setCurrentProgram(int)
{
}

const juce::String RGainAudioProcessor::getProgramName(int)
{
    return {};
}

void RGainAudioProcessor::changeProgramName(int, const juce::String&)
{
}

void RGainAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    if (auto state = apvts.copyState(); state.isValid())
    {
        if (auto xml = state.createXml())
            copyXmlToBinary(*xml, destData);
    }
}

void RGainAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
    {
        if (xml->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

float RGainAudioProcessor::getLeftMeterLevel() const noexcept
{
    return leftMeterLevel.load(std::memory_order_relaxed);
}

float RGainAudioProcessor::getRightMeterLevel() const noexcept
{
    return rightMeterLevel.load(std::memory_order_relaxed);
}

void RGainAudioProcessor::updateOutputMeter(const juce::AudioBuffer<float>& buffer) noexcept
{
    const auto numSamples = buffer.getNumSamples();

    if (buffer.getNumChannels() == 0 || numSamples == 0)
    {
        leftMeterLevel.store(0.0f, std::memory_order_relaxed);
        rightMeterLevel.store(0.0f, std::memory_order_relaxed);
        return;
    }

    const auto leftPeak = buffer.getMagnitude(0, 0, numSamples);
    const auto rightPeak = buffer.getNumChannels() > 1
        ? buffer.getMagnitude(1, 0, numSamples)
        : leftPeak;

    leftMeterLevel.store(juce::jlimit(0.0f, 1.0f, leftPeak), std::memory_order_relaxed);
    rightMeterLevel.store(juce::jlimit(0.0f, 1.0f, rightPeak), std::memory_order_relaxed);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RGainAudioProcessor();
}
