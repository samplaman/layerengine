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

    struct FXParams {
        float chorusRate = 1.0f;
        float chorusDepth = 0.2f;
        float chorusMix = 0.0f;
        
        float filterCutoff = 20000.0f;
        float filterResonance = 0.1f;
        
        float limiterThreshold = 0.0f; // dB
        float limiterRelease = 100.0f; // ms
    };
    FXParams& getFXParams() { return fxParams; }

    float getPitchBend() const { return lastPitchBend; }
    void setPitchBend(float val) {
        lastPitchBend = val;
        for (auto& layer : layers)
            layer.getParams().pitchBend = val;
    }

    float getModulation() const { return lastModulation; }
    void setModulation(float val) {
        lastModulation = val;
        for (auto& layer : layers)
            layer.getParams().position = val;
    }

private:
    std::array<GranularLayer, 4> layers;
    juce::MidiKeyboardState keyboardState;
    
    float lastPitchBend = 0.0f;
    float lastModulation = 0.0f;
    
    // Global Effects
    juce::dsp::Reverb reverb;
    juce::dsp::Reverb::Parameters reverbParams;
    
    juce::dsp::Chorus<float> chorus;
    juce::dsp::StateVariableTPTFilter<float> masterFilter;
    juce::dsp::Limiter<float> limiter;
    FXParams fxParams;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranularSynthAudioProcessor)
};
