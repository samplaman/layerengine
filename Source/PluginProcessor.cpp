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
    
    if (!fxParams.chorusBypass && fxParams.chorusMix > 0.01f)
        chorus.process(context);
        
    if (!fxParams.reverbBypass)
        reverb.process(context);
    
    if (!fxParams.filterBypass && fxParams.filterCutoff < 19999.0f) {
        masterFilter.process(context);
    }
    
    if (!fxParams.limiterBypass)
        limiter.process(context);
}

juce::AudioProcessorEditor* GranularSynthAudioProcessor::createEditor()
{
    return new GranularSynthAudioProcessorEditor (*this);
}

void GranularSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Create an XML element to store our settings
    juce::XmlElement xmlState ("LayerEngineState");
    
    // 1. Global FX parameters
    auto* xmlGlobal = xmlState.createNewChildElement ("GlobalFX");
    xmlGlobal->setAttribute ("reverbSize", reverbParams.roomSize);
    xmlGlobal->setAttribute ("reverbWet", reverbParams.wetLevel);
    xmlGlobal->setAttribute ("reverbBypass", fxParams.reverbBypass);
    xmlGlobal->setAttribute ("chorusRate", fxParams.chorusRate);
    xmlGlobal->setAttribute ("chorusDepth", fxParams.chorusDepth);
    xmlGlobal->setAttribute ("chorusMix", fxParams.chorusMix);
    xmlGlobal->setAttribute ("chorusBypass", fxParams.chorusBypass);
    xmlGlobal->setAttribute ("filterCutoff", fxParams.filterCutoff);
    xmlGlobal->setAttribute ("filterResonance", fxParams.filterResonance);
    xmlGlobal->setAttribute ("filterBypass", fxParams.filterBypass);
    xmlGlobal->setAttribute ("limiterThreshold", fxParams.limiterThreshold);
    xmlGlobal->setAttribute ("limiterRelease", fxParams.limiterRelease);
    xmlGlobal->setAttribute ("limiterBypass", fxParams.limiterBypass);

    // 2. Layer parameters
    for (int i = 0; i < 4; ++i)
    {
        auto& layer = layers[i];
        auto& p = layer.getParams();
        
        auto* xmlLayer = xmlState.createNewChildElement ("Layer");
        xmlLayer->setAttribute ("index", i);
        xmlLayer->setAttribute ("samplePath", layer.getCurrentSamplePath());
        
        xmlLayer->setAttribute ("position", p.position);
        xmlLayer->setAttribute ("size", p.size);
        xmlLayer->setAttribute ("density", p.density);
        xmlLayer->setAttribute ("pitch", p.pitch);
        xmlLayer->setAttribute ("volume", p.volume);
        
        xmlLayer->setAttribute ("posRandomness", p.posRandomness);
        xmlLayer->setAttribute ("pitchRandomness", p.pitchRandomness);
        xmlLayer->setAttribute ("sizeRandomness", p.sizeRandomness);
        
        xmlLayer->setAttribute ("pan", p.pan);
        xmlLayer->setAttribute ("spread", p.spread);
        
        xmlLayer->setAttribute ("scanSpeed", p.scanSpeed);
        xmlLayer->setAttribute ("reverseProbability", p.reverseProbability);
        xmlLayer->setAttribute ("windowShape", (int)p.windowShape);
        
        xmlLayer->setAttribute ("attack", p.attack);
        xmlLayer->setAttribute ("decay", p.decay);
        xmlLayer->setAttribute ("sustain", p.sustain);
        xmlLayer->setAttribute ("release", p.release);
        
        xmlLayer->setAttribute ("filterCutoff", p.filterCutoff);
        xmlLayer->setAttribute ("filterResonance", p.filterResonance);
        
        xmlLayer->setAttribute ("isMuted", p.isMuted);
        xmlLayer->setAttribute ("isSoloed", p.isSoloed);
    }
    
    // Save to memory
    copyXmlToBinary (xmlState, destData);
}

void GranularSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState != nullptr && xmlState->hasTagName ("LayerEngineState"))
    {
        // 1. Restore Global FX
        if (auto* xmlGlobal = xmlState->getChildByName ("GlobalFX"))
        {
            reverbParams.roomSize = (float)xmlGlobal->getDoubleAttribute ("reverbSize", reverbParams.roomSize);
            reverbParams.wetLevel = (float)xmlGlobal->getDoubleAttribute ("reverbWet", reverbParams.wetLevel);
            fxParams.reverbBypass = xmlGlobal->getBoolAttribute ("reverbBypass", fxParams.reverbBypass);
            reverb.setParameters (reverbParams);
            
            fxParams.chorusRate = (float)xmlGlobal->getDoubleAttribute ("chorusRate", fxParams.chorusRate);
            fxParams.chorusDepth = (float)xmlGlobal->getDoubleAttribute ("chorusDepth", fxParams.chorusDepth);
            fxParams.chorusMix = (float)xmlGlobal->getDoubleAttribute ("chorusMix", fxParams.chorusMix);
            fxParams.chorusBypass = xmlGlobal->getBoolAttribute ("chorusBypass", fxParams.chorusBypass);
            
            fxParams.filterCutoff = (float)xmlGlobal->getDoubleAttribute ("filterCutoff", fxParams.filterCutoff);
            fxParams.filterResonance = (float)xmlGlobal->getDoubleAttribute ("filterResonance", fxParams.filterResonance);
            fxParams.filterBypass = xmlGlobal->getBoolAttribute ("filterBypass", fxParams.filterBypass);
            
            fxParams.limiterThreshold = (float)xmlGlobal->getDoubleAttribute ("limiterThreshold", fxParams.limiterThreshold);
            fxParams.limiterRelease = (float)xmlGlobal->getDoubleAttribute ("limiterRelease", fxParams.limiterRelease);
            fxParams.limiterBypass = xmlGlobal->getBoolAttribute ("limiterBypass", fxParams.limiterBypass);
        }
        
        // 2. Restore Layers
        for (auto* xmlLayer : xmlState->getChildIterator())
        {
            if (xmlLayer->hasTagName ("Layer"))
            {
                int i = xmlLayer->getIntAttribute ("index", -1);
                if (i >= 0 && i < 4)
                {
                    auto& layer = layers[i];
                    auto& p = layer.getParams();
                    
                    p.position = (float)xmlLayer->getDoubleAttribute ("position", p.position);
                    p.size = (float)xmlLayer->getDoubleAttribute ("size", p.size);
                    p.density = (float)xmlLayer->getDoubleAttribute ("density", p.density);
                    p.pitch = (float)xmlLayer->getDoubleAttribute ("pitch", p.pitch);
                    p.volume = (float)xmlLayer->getDoubleAttribute ("volume", p.volume);
                    
                    p.posRandomness = (float)xmlLayer->getDoubleAttribute ("posRandomness", p.posRandomness);
                    p.pitchRandomness = (float)xmlLayer->getDoubleAttribute ("pitchRandomness", p.pitchRandomness);
                    p.sizeRandomness = (float)xmlLayer->getDoubleAttribute ("sizeRandomness", p.sizeRandomness);
                    
                    p.pan = (float)xmlLayer->getDoubleAttribute ("pan", p.pan);
                    p.spread = (float)xmlLayer->getDoubleAttribute ("spread", p.spread);
                    
                    p.scanSpeed = (float)xmlLayer->getDoubleAttribute ("scanSpeed", p.scanSpeed);
                    p.reverseProbability = (float)xmlLayer->getDoubleAttribute ("reverseProbability", p.reverseProbability);
                    p.windowShape = (WindowShape)xmlLayer->getIntAttribute ("windowShape", (int)p.windowShape);
                    
                    p.attack = (float)xmlLayer->getDoubleAttribute ("attack", p.attack);
                    p.decay = (float)xmlLayer->getDoubleAttribute ("decay", p.decay);
                    p.sustain = (float)xmlLayer->getDoubleAttribute ("sustain", p.sustain);
                    p.release = (float)xmlLayer->getDoubleAttribute ("release", p.release);
                    
                    p.filterCutoff = (float)xmlLayer->getDoubleAttribute ("filterCutoff", p.filterCutoff);
                    p.filterResonance = (float)xmlLayer->getDoubleAttribute ("filterResonance", p.filterResonance);
                    
                    p.isMuted = xmlLayer->getBoolAttribute ("isMuted", p.isMuted);
                    p.isSoloed = xmlLayer->getBoolAttribute ("isSoloed", p.isSoloed);
                    
                    // Restore Sample Path
                    juce::String samplePath = xmlLayer->getStringAttribute ("samplePath");
                    if (samplePath.isNotEmpty())
                    {
                        juce::File file (samplePath);
                        if (file.existsAsFile())
                        {
                            layer.loadSample (file);
                        }
                    }
                    else
                    {
                        layer.clearSample();
                    }
                }
            }
        }
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GranularSynthAudioProcessor();
}
