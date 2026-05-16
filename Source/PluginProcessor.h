#pragma once

#include <JuceHeader.h>
#include "GranularLayer.h"

class GranularSynthAudioProcessor  : public juce::AudioProcessor
{
public:
    GranularSynthAudioProcessor();
    ~GranularSynthAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Granular Synth"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int index) override {}
    const juce::String getProgramName (int index) override { return {}; }
    void changeProgramName (int index, const juce::String& newName) override {}

    void getStateInformation (juce::MemoryBlock& destData) override {}
    void setStateInformation (const void* data, int sizeInBytes) override {}

    GranularLayer& getLayer(int index) { return layers[index]; }
    juce::MidiKeyboardState& getKeyboardState() { return keyboardState; }
    
    // Effects
    juce::dsp::Reverb& getReverb() { return reverb; }
    juce::dsp::Reverb::Parameters& getReverbParams() { return reverbParams; }

private:
    std::array<GranularLayer, 4> layers;
    juce::MidiKeyboardState keyboardState;
    
    // Global Effects
    juce::dsp::Reverb reverb;
    juce::dsp::Reverb::Parameters reverbParams;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranularSynthAudioProcessor)
};
