#include "PluginProcessor.h"
#include "PluginEditor.h"

GranularSynthAudioProcessor::GranularSynthAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
    reverbParams.roomSize = 0.5f;
    reverbParams.damping = 0.5f;
    reverbParams.wetLevel = 0.33f;
    reverbParams.dryLevel = 1.0f;
    reverbParams.width = 1.0f;
    reverb.setParameters(reverbParams);
}

GranularSynthAudioProcessor::~GranularSynthAudioProcessor() {}

void GranularSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (auto& layer : layers)
        layer.prepareToPlay(sampleRate, samplesPerBlock);
    
    keyboardState.reset();

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = (juce::uint32)samplesPerBlock;
    spec.numChannels = 2;
    
    reverb.prepare(spec);
    chorus.prepare(spec);
    masterFilter.prepare(spec);
    masterFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    limiter.prepare(spec);
}

void GranularSynthAudioProcessor::releaseResources() {}

void GranularSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    for (const auto metadata : midiMessages)
    {
        auto msg = metadata.getMessage();
        if (msg.isPitchWheel())
        {
            lastPitchBend = (float)(msg.getPitchWheelValue() - 8192) / 8192.0f;
        }
        else if (msg.isController() && msg.getControllerNumber() == 1)
        {
            lastModulation = (float)msg.getControllerValue() / 127.0f;
        }
    }

    bool anySoloed = false;
    for (auto& layer : layers)
    {
        if (layer.getParams().isSoloed)
        {
            anySoloed = true;
            break;
        }
    }

    for (auto& layer : layers)
    {
        float gain = 1.0f;
        if (layer.getParams().isMuted)
        {
            gain = 0.0f;
        }
        else if (anySoloed && !layer.getParams().isSoloed)
        {
            gain = 0.0f;
        }
        
        layer.processBlock(buffer, midiMessages, gain);
    }

    // Apply Global Effects
    reverb.setParameters(reverbParams);
    
    chorus.setRate(fxParams.chorusRate);
    chorus.setDepth(fxParams.chorusDepth);
    chorus.setMix(fxParams.chorusMix);

    masterFilter.setCutoffFrequency(fxParams.filterCutoff);
    masterFilter.setResonance(fxParams.filterResonance);
    
    limiter.setThreshold(fxParams.limiterThreshold);
    limiter.setRelease(fxParams.limiterRelease);

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    
    if (fxParams.chorusMix > 0.01f)
        chorus.process(context);
        
    reverb.process(context);
    
    if (fxParams.filterCutoff < 19999.0f) {
        masterFilter.process(context);
    }
    
    limiter.process(context);
}

juce::AudioProcessorEditor* GranularSynthAudioProcessor::createEditor()
{
    return new GranularSynthAudioProcessorEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GranularSynthAudioProcessor();
}
