#include "GranularLayer.h"

GranularLayer::GranularLayer()
{
    formatManager.registerBasicFormats();
    grains.resize(300);
}

GranularLayer::~GranularLayer() {}

void GranularLayer::loadSample(const juce::File& file)
{
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
    if (reader != nullptr)
    {
        juce::AudioSampleBuffer tempBuffer;
        tempBuffer.setSize(reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&tempBuffer, 0, (int)reader->lengthInSamples, 0, true, true);

        const juce::ScopedLock sl(lock);
        sampleBuffer.makeCopyOf(tempBuffer);
        
        for (auto& grain : grains)
            grain.active = false;
            
        adsr.reset();
        activeNoteNumbers.clear();
        currentSamplePath = file.getFullPathName();
    }
}

void GranularLayer::clearSample()
{
    const juce::ScopedLock sl(lock);
    sampleBuffer.setSize(0, 0);
    
    for (auto& grain : grains)
        grain.active = false;
        
    adsr.reset();
    activeNoteNumbers.clear();
    currentSamplePath = {};
}

void GranularLayer::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    adsr.setSampleRate(sampleRate);
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = (juce::uint32)samplesPerBlock;
    spec.numChannels = 2;
    
    filter.prepare(spec);
    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
}

void GranularLayer::handleMidi(juce::MidiBuffer& midiMessages)
{
    for (const auto metadata : midiMessages)
    {
        auto msg = metadata.getMessage();
        if (msg.isNoteOn())
        {
            activeNoteNumbers.insert(msg.getNoteNumber());
            lastNoteNumber = msg.getNoteNumber();
            adsr.noteOn();
        }
        else if (msg.isNoteOff())
        {
            activeNoteNumbers.erase(msg.getNoteNumber());
            if (activeNoteNumbers.empty())
                adsr.noteOff();
        }
        else if (msg.isController())
        {
            if (msg.getControllerNumber() == 1)
                params.position = (float)msg.getControllerValue() / 127.0f;
        }
        else if (msg.isPitchWheel())
        {
            params.pitchBend = (float)(msg.getPitchWheelValue() - 8192) / 8192.0f;
        }
    }
}

static float getEnvelopeValue(double phase, WindowShape shape)
{
    switch (shape)
    {
        case WindowShape::Sine:
            return std::sin(phase * juce::MathConstants<double>::pi);
        case WindowShape::Hann:
            return 0.5f * (1.0f - std::cos(2.0 * juce::MathConstants<double>::pi * phase));
        case WindowShape::Gaussian:
        {
            double x = (phase - 0.5) * 6.0; // 6 sigma
            return std::exp(-0.5 * x * x);
        }
        case WindowShape::Triangular:
            return 1.0f - std::abs(2.0f * (float)phase - 1.0f);
    }
    return 1.0f;
}

static float getInterpolatedSample(const juce::AudioSampleBuffer& buffer, int channel, double sampleIdx)
{
    int numSamples = buffer.getNumSamples();
    if (numSamples == 0) return 0.0f;
    
    int idx1 = (int)sampleIdx;
    int idx0 = idx1 - 1;
    int idx2 = idx1 + 1;
    int idx3 = idx1 + 2;
    
    float frac = (float)(sampleIdx - idx1);
    
    if (idx1 < 0 || idx1 >= numSamples) return 0.0f;
    
    // Boundary clamping for 4-point Hermite interpolation
    float s0 = (idx0 >= 0) ? buffer.getSample(channel, idx0) : buffer.getSample(channel, idx1);
    float s1 = buffer.getSample(channel, idx1);
    float s2 = (idx2 < numSamples) ? buffer.getSample(channel, idx2) : s1;
    float s3 = (idx3 < numSamples) ? buffer.getSample(channel, idx3) : s2;
    
    // Cubic Hermite spline coefficients
    float c0 = s1;
    float c1 = 0.5f * (s2 - s0);
    float c2 = s0 - 2.5f * s1 + 2.0f * s2 - 0.5f * s3;
    float c3 = 0.5f * (s3 - s0) + 1.5f * (s1 - s2);
    
    return ((c3 * frac + c2) * frac + c1) * frac + c0;
}

void GranularLayer::spawnGrain()
{
    if (sampleBuffer.getNumSamples() == 0) return;

    auto& random = juce::Random::getSystemRandom();

    for (auto& grain : grains)
    {
        if (!grain.active)
        {
            // Position with randomness
            float randPos = (random.nextFloat() - 0.5f) * params.posRandomness;
            float pos = juce::jlimit(0.0f, 1.0f, params.position + randPos);
            grain.startSample = pos * (sampleBuffer.getNumSamples() - 1);
            
            // Size with randomness
            float randSize = 1.0f + (random.nextFloat() - 0.5f) * params.sizeRandomness;
            grain.length = params.size * currentSampleRate * randSize;
            
            grain.currentOffset = 0;
            grain.amplitude = params.volume;
            
            // Pitch with randomness and MIDI
            int noteToUse = lastNoteNumber;
            if (!activeNoteNumbers.empty())
            {
                auto it = activeNoteNumbers.begin();
                std::advance(it, random.nextInt((int)activeNoteNumbers.size()));
                noteToUse = *it;
            }
            
            float randPitch = (random.nextFloat() - 0.5f) * params.pitchRandomness;
            float totalPitchOffset = (noteToUse - 60) + (params.pitchBend * 2.0f) + randPitch;
            float notePitchFactor = std::pow(2.0f, totalPitchOffset / 12.0f);
            grain.pitch = params.pitch * notePitchFactor;
            
            // Reverse
            grain.isReverse = random.nextFloat() < params.reverseProbability;
            
            // Panning
            float grainPan = juce::jlimit(-1.0f, 1.0f, params.pan + (random.nextFloat() - 0.5f) * params.spread);
            grain.panL = std::cos((grainPan + 1.0f) * 0.25f * juce::MathConstants<float>::pi);
            grain.panR = std::sin((grainPan + 1.0f) * 0.25f * juce::MathConstants<float>::pi);

            grain.active = true;
            break;
        }
    }
}

void GranularLayer::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages, float gain)
{
    const juce::ScopedTryLock sl(lock);
    if (!sl.isLocked()) return;

    handleMidi(midiMessages);

    if (sampleBuffer.getNumSamples() == 0) return;

    int numSamples = buffer.getNumSamples();
    
    // Update playhead (Scanning)
    if (params.scanSpeed != 0.0f)
    {
        params.position += (params.scanSpeed / (float)currentSampleRate) * numSamples;
        while (params.position > 1.0f) params.position -= 1.0f;
        while (params.position < 0.0f) params.position += 1.0f;
    }

    juce::ADSR::Parameters adsrParams;
    adsrParams.attack = params.attack;
    adsrParams.decay = params.decay;
    adsrParams.sustain = params.sustain;
    adsrParams.release = params.release;
    adsr.setParameters(adsrParams);

    if (adsr.isActive())
    {
        float samplesPerGrain = (float)currentSampleRate / params.density;
        nextGrainCounter -= numSamples;
        
        while (nextGrainCounter <= 0)
        {
            spawnGrain();
            nextGrainCounter += (int)samplesPerGrain;
        }
    }

    juce::AudioBuffer<float> layerBuffer(2, numSamples);
    layerBuffer.clear();

    auto* outL = layerBuffer.getWritePointer(0);
    auto* outR = layerBuffer.getWritePointer(1);

    for (auto& grain : grains)
    {
        if (grain.active)
        {
            for (int s = 0; s < numSamples; ++s)
            {
                double phase = grain.currentOffset / grain.length;
                if (phase >= 1.0)
                {
                    grain.active = false;
                    break;
                }

                double playbackOffset = grain.isReverse ? (grain.length - grain.currentOffset) : grain.currentOffset;
                double sampleIdx = grain.startSample + (playbackOffset * grain.pitch);
                
                // Wrap or clamp sample index if needed, but here we just stop if out of bounds
                if (sampleIdx < 0 || sampleIdx >= sampleBuffer.getNumSamples())
                {
                    grain.active = false;
                    break;
                }

                float env = getEnvelopeValue(phase, params.windowShape);
                float sampleL = getInterpolatedSample(sampleBuffer, 0, sampleIdx);
                float sampleR = (sampleBuffer.getNumChannels() > 1) ? getInterpolatedSample(sampleBuffer, 1, sampleIdx) : sampleL;
                
                float monoSample = (sampleL + sampleR) * 0.5f;
                float finalSample = monoSample * grain.amplitude * env;

                outL[s] += finalSample * grain.panL;
                outR[s] += finalSample * grain.panR;

                grain.currentOffset += 1.0;
            }
        }
    }

    // Apply ADSR
    adsr.applyEnvelopeToBuffer(layerBuffer, 0, numSamples);

    // Apply Filter
    filter.setCutoffFrequency(params.filterCutoff);
    filter.setResonance(params.filterResonance);
    
    juce::dsp::AudioBlock<float> block(layerBuffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    filter.process(context);

    // Calculate peak level for mixer meters
    float maxSample = 0.0f;
    for (int ch = 0; ch < layerBuffer.getNumChannels(); ++ch)
    {
        auto* readPtr = layerBuffer.getReadPointer(ch);
        for (int s = 0; s < numSamples; ++s)
        {
            float absVal = std::abs(readPtr[s]);
            if (absVal > maxSample)
                maxSample = absVal;
        }
    }
    currentLevel.store(maxSample);

    // Add to main buffer
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        buffer.addFrom(ch, 0, layerBuffer, ch % 2, 0, numSamples, gain);
}
