#include "PluginProcessor.h"
#include "PluginEditor.h"

GranularSynthAudioProcessor::GranularSynthAudioProcessor()
    : AudioProcessor(BusesProperties().withOutput(
          "Output", juce::AudioChannelSet::stereo(), true)) {
  reverbParams.roomSize = 0.5f;
  reverbParams.damping = 0.5f;
  reverbParams.wetLevel = 0.33f;
  reverbParams.dryLevel = 1.0f;
  reverbParams.width = 1.0f;
  reverb.setParameters(reverbParams);
}

GranularSynthAudioProcessor::~GranularSynthAudioProcessor() {}

void GranularSynthAudioProcessor::prepareToPlay(double sampleRate,
                                                int samplesPerBlock) {
  for (auto &layer : layers)
    layer.prepareToPlay(sampleRate, samplesPerBlock);

  keyboardState.reset();

  juce::dsp::ProcessSpec spec;
  spec.sampleRate = sampleRate;
  spec.maximumBlockSize = (juce::uint32)samplesPerBlock;
  spec.numChannels = 2;

  reverb.prepare(spec);
  chorus.prepare(spec);
  delay.prepare(spec);
  phaser.prepare(spec);
  masterFilter.prepare(spec);
  masterFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
  limiter.prepare(spec);
}

void GranularSynthAudioProcessor::releaseResources() {}

void GranularSynthAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                               juce::MidiBuffer &midiMessages) {
  juce::ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(),
                                      true);

  for (const auto metadata : midiMessages) {
    auto msg = metadata.getMessage();
    if (msg.isPitchWheel()) {
      lastPitchBend = (float)(msg.getPitchWheelValue() - 8192) / 8192.0f;
    } else if (msg.isController()) {
      int ccNumber = msg.getControllerNumber();
      float ccNormalized = (float)msg.getControllerValue() / 127.0f;

      juce::ScopedLock sl(midiLock);

      // If in MIDI learn mode, map the touched controller to this parameter
      if (learningParamID.isNotEmpty()) {
        midiMappings[learningParamID] = ccNumber;
        learningParamID = ""; // Exit learn mode
      }

      // Apply controller value to all mapped parameters
      for (auto const &[paramId, mappedCC] : midiMappings) {
        if (mappedCC == ccNumber) {
          float mappedVal = mapCCValueToParam(paramId, ccNormalized);
          setParameterValue(paramId, mappedVal);
        }
      }

      if (ccNumber == 1) {
        lastModulation = ccNormalized;
      }
    }
  }

  bool anySoloed = false;
  for (auto &layer : layers) {
    if (layer.getParams().isSoloed) {
      anySoloed = true;
      break;
    }
  }

  for (auto &layer : layers) {
    float gain = 1.0f;
    if (layer.getParams().isMuted) {
      gain = 0.0f;
    } else if (anySoloed && !layer.getParams().isSoloed) {
      gain = 0.0f;
    }

    layer.processBlock(buffer, midiMessages, gain);
  }

  // Apply Global Effects
  reverb.setParameters(reverbParams);

  chorus.setRate(fxParams.chorusRate);
  chorus.setDepth(fxParams.chorusDepth);
  chorus.setMix(fxParams.chorusMix);

  phaser.setRate(fxParams.phaserRate);
  phaser.setDepth(fxParams.phaserDepth);
  phaser.setCentreFrequency(fxParams.phaserCenterFreq);
  phaser.setFeedback(0.5f);
  phaser.setMix(fxParams.phaserMix);

  masterFilter.setCutoffFrequency(fxParams.filterCutoff);
  masterFilter.setResonance(fxParams.filterResonance);

  limiter.setThreshold(fxParams.limiterThreshold);
  limiter.setRelease(fxParams.limiterRelease);

  juce::dsp::AudioBlock<float> block(buffer);
  juce::dsp::ProcessContextReplacing<float> context(block);

  if (!fxParams.chorusBypass && fxParams.chorusMix > 0.01f)
    chorus.process(context);

  if (!fxParams.phaserBypass && fxParams.phaserMix > 0.01f)
    phaser.process(context);

  if (!fxParams.delayBypass && fxParams.delayMix > 0.01f) {
    float delayInSamples =
        juce::jmax(0.1f, fxParams.delayTime * (float)getSampleRate() / 1000.0f);
    for (size_t channel = 0; channel < block.getNumChannels(); ++channel) {
      auto *channelData = block.getChannelPointer(channel);
      for (size_t i = 0; i < block.getNumSamples(); ++i) {
        float input = channelData[i];
        float delayed = delay.popSample((int)channel, delayInSamples);
        delay.pushSample((int)channel,
                         input + delayed * fxParams.delayFeedback);
        channelData[i] =
            input * (1.0f - fxParams.delayMix) + delayed * fxParams.delayMix;
      }
    }
  } else {
    for (size_t channel = 0; channel < block.getNumChannels(); ++channel) {
      auto *channelData = block.getChannelPointer(channel);
      for (size_t i = 0; i < block.getNumSamples(); ++i) {
        delay.pushSample((int)channel,
                         channelData[i]); // Keep delay buffer warm
        delay.popSample((int)channel, 0.1f);
      }
    }
  }

  if (!fxParams.reverbBypass)
    reverb.process(context);

  if (!fxParams.filterBypass && fxParams.filterCutoff < 19999.0f) {
    masterFilter.process(context);
  }

  if (!fxParams.limiterBypass)
    limiter.process(context);
}

juce::AudioProcessorEditor *GranularSynthAudioProcessor::createEditor() {
  return new GranularSynthAudioProcessorEditor(*this);
}

void GranularSynthAudioProcessor::getStateInformation(
    juce::MemoryBlock &destData) {
  // Create an XML element to store our settings
  juce::XmlElement xmlState("LayerEngineState");

  // 1. Global FX parameters
  auto *xmlGlobal = xmlState.createNewChildElement("GlobalFX");
  xmlGlobal->setAttribute("reverbSize", reverbParams.roomSize);
  xmlGlobal->setAttribute("reverbWet", reverbParams.wetLevel);
  xmlGlobal->setAttribute("reverbBypass", fxParams.reverbBypass);
  xmlGlobal->setAttribute("chorusRate", fxParams.chorusRate);
  xmlGlobal->setAttribute("chorusDepth", fxParams.chorusDepth);
  xmlGlobal->setAttribute("chorusMix", fxParams.chorusMix);
  xmlGlobal->setAttribute("chorusBypass", fxParams.chorusBypass);
  xmlGlobal->setAttribute("delayTime", fxParams.delayTime);
  xmlGlobal->setAttribute("delayFeedback", fxParams.delayFeedback);
  xmlGlobal->setAttribute("delayMix", fxParams.delayMix);
  xmlGlobal->setAttribute("delayBypass", fxParams.delayBypass);
  xmlGlobal->setAttribute("phaserRate", fxParams.phaserRate);
  xmlGlobal->setAttribute("phaserDepth", fxParams.phaserDepth);
  xmlGlobal->setAttribute("phaserCenterFreq", fxParams.phaserCenterFreq);
  xmlGlobal->setAttribute("phaserMix", fxParams.phaserMix);
  xmlGlobal->setAttribute("phaserBypass", fxParams.phaserBypass);
  xmlGlobal->setAttribute("filterCutoff", fxParams.filterCutoff);
  xmlGlobal->setAttribute("filterResonance", fxParams.filterResonance);
  xmlGlobal->setAttribute("filterBypass", fxParams.filterBypass);
  xmlGlobal->setAttribute("limiterThreshold", fxParams.limiterThreshold);
  xmlGlobal->setAttribute("limiterRelease", fxParams.limiterRelease);
  xmlGlobal->setAttribute("limiterBypass", fxParams.limiterBypass);

  // 2. Layer parameters
  for (int i = 0; i < 4; ++i) {
    auto &layer = layers[i];
    auto &p = layer.getParams();

    auto *xmlLayer = xmlState.createNewChildElement("Layer");
    xmlLayer->setAttribute("index", i);
    xmlLayer->setAttribute("samplePath", layer.getCurrentSamplePath());

    xmlLayer->setAttribute("position", p.position);
    xmlLayer->setAttribute("size", p.size);
    xmlLayer->setAttribute("density", p.density);
    xmlLayer->setAttribute("pitch", p.pitch);
    xmlLayer->setAttribute("volume", p.volume);

    xmlLayer->setAttribute("posRandomness", p.posRandomness);
    xmlLayer->setAttribute("pitchRandomness", p.pitchRandomness);
    xmlLayer->setAttribute("sizeRandomness", p.sizeRandomness);

    xmlLayer->setAttribute("pan", p.pan);
    xmlLayer->setAttribute("spread", p.spread);

    xmlLayer->setAttribute("scanSpeed", p.scanSpeed);
    xmlLayer->setAttribute("reverseProbability", p.reverseProbability);
    xmlLayer->setAttribute("windowShape", (int)p.windowShape);

    xmlLayer->setAttribute("attack", p.attack);
    xmlLayer->setAttribute("decay", p.decay);
    xmlLayer->setAttribute("sustain", p.sustain);
    xmlLayer->setAttribute("release", p.release);

    xmlLayer->setAttribute("filterCutoff", p.filterCutoff);
    xmlLayer->setAttribute("filterResonance", p.filterResonance);

    xmlLayer->setAttribute("lowNote", p.lowNote);
    xmlLayer->setAttribute("highNote", p.highNote);

    xmlLayer->setAttribute("isMuted", p.isMuted);
    xmlLayer->setAttribute("isSoloed", p.isSoloed);
  }

  // 3. MIDI Mappings
  auto *xmlMappings = xmlState.createNewChildElement("MidiMappings");
  juce::ScopedLock sl(midiLock);
  for (auto const &[paramId, ccNumber] : midiMappings) {
    auto *xmlMap = xmlMappings->createNewChildElement("Map");
    xmlMap->setAttribute("paramId", paramId);
    xmlMap->setAttribute("ccNumber", ccNumber);
  }

  // Save to memory
  copyXmlToBinary(xmlState, destData);
}

void GranularSynthAudioProcessor::setStateInformation(const void *data,
                                                      int sizeInBytes) {
  std::unique_ptr<juce::XmlElement> xmlState(
      getXmlFromBinary(data, sizeInBytes));

  if (xmlState != nullptr && xmlState->hasTagName("LayerEngineState")) {
    // 1. Restore Global FX
    if (auto *xmlGlobal = xmlState->getChildByName("GlobalFX")) {
      reverbParams.roomSize = (float)xmlGlobal->getDoubleAttribute(
          "reverbSize", reverbParams.roomSize);
      reverbParams.wetLevel = (float)xmlGlobal->getDoubleAttribute(
          "reverbWet", reverbParams.wetLevel);
      fxParams.reverbBypass =
          xmlGlobal->getBoolAttribute("reverbBypass", fxParams.reverbBypass);
      reverb.setParameters(reverbParams);

      fxParams.chorusRate = (float)xmlGlobal->getDoubleAttribute(
          "chorusRate", fxParams.chorusRate);
      fxParams.chorusDepth = (float)xmlGlobal->getDoubleAttribute(
          "chorusDepth", fxParams.chorusDepth);
      fxParams.chorusMix =
          (float)xmlGlobal->getDoubleAttribute("chorusMix", fxParams.chorusMix);
      fxParams.chorusBypass =
          xmlGlobal->getBoolAttribute("chorusBypass", fxParams.chorusBypass);

      fxParams.delayTime =
          (float)xmlGlobal->getDoubleAttribute("delayTime", fxParams.delayTime);
      fxParams.delayFeedback = (float)xmlGlobal->getDoubleAttribute(
          "delayFeedback", fxParams.delayFeedback);
      fxParams.delayMix =
          (float)xmlGlobal->getDoubleAttribute("delayMix", fxParams.delayMix);
      fxParams.delayBypass =
          xmlGlobal->getBoolAttribute("delayBypass", fxParams.delayBypass);

      fxParams.phaserRate = (float)xmlGlobal->getDoubleAttribute(
          "phaserRate", fxParams.phaserRate);
      fxParams.phaserDepth = (float)xmlGlobal->getDoubleAttribute(
          "phaserDepth", fxParams.phaserDepth);
      fxParams.phaserCenterFreq = (float)xmlGlobal->getDoubleAttribute(
          "phaserCenterFreq", fxParams.phaserCenterFreq);
      fxParams.phaserMix =
          (float)xmlGlobal->getDoubleAttribute("phaserMix", fxParams.phaserMix);
      fxParams.phaserBypass =
          xmlGlobal->getBoolAttribute("phaserBypass", fxParams.phaserBypass);

      fxParams.filterCutoff = (float)xmlGlobal->getDoubleAttribute(
          "filterCutoff", fxParams.filterCutoff);
      fxParams.filterResonance = (float)xmlGlobal->getDoubleAttribute(
          "filterResonance", fxParams.filterResonance);
      fxParams.filterBypass =
          xmlGlobal->getBoolAttribute("filterBypass", fxParams.filterBypass);

      fxParams.limiterThreshold = (float)xmlGlobal->getDoubleAttribute(
          "limiterThreshold", fxParams.limiterThreshold);
      fxParams.limiterRelease = (float)xmlGlobal->getDoubleAttribute(
          "limiterRelease", fxParams.limiterRelease);
      fxParams.limiterBypass =
          xmlGlobal->getBoolAttribute("limiterBypass", fxParams.limiterBypass);
    }

    // 2. Restore Layers
    for (auto *xmlLayer : xmlState->getChildIterator()) {
      if (xmlLayer->hasTagName("Layer")) {
        int i = xmlLayer->getIntAttribute("index", -1);
        if (i >= 0 && i < 4) {
          auto &layer = layers[i];
          auto &p = layer.getParams();

          p.position =
              (float)xmlLayer->getDoubleAttribute("position", p.position);
          p.size = (float)xmlLayer->getDoubleAttribute("size", p.size);
          p.density = (float)xmlLayer->getDoubleAttribute("density", p.density);
          p.pitch = (float)xmlLayer->getDoubleAttribute("pitch", p.pitch);
          p.volume = (float)xmlLayer->getDoubleAttribute("volume", p.volume);

          p.posRandomness = (float)xmlLayer->getDoubleAttribute(
              "posRandomness", p.posRandomness);
          p.pitchRandomness = (float)xmlLayer->getDoubleAttribute(
              "pitchRandomness", p.pitchRandomness);
          p.sizeRandomness = (float)xmlLayer->getDoubleAttribute(
              "sizeRandomness", p.sizeRandomness);

          p.pan = (float)xmlLayer->getDoubleAttribute("pan", p.pan);
          p.spread = (float)xmlLayer->getDoubleAttribute("spread", p.spread);

          p.scanSpeed =
              (float)xmlLayer->getDoubleAttribute("scanSpeed", p.scanSpeed);
          p.reverseProbability = (float)xmlLayer->getDoubleAttribute(
              "reverseProbability", p.reverseProbability);
          p.windowShape = (WindowShape)xmlLayer->getIntAttribute(
              "windowShape", (int)p.windowShape);

          p.attack = (float)xmlLayer->getDoubleAttribute("attack", p.attack);
          p.decay = (float)xmlLayer->getDoubleAttribute("decay", p.decay);
          p.sustain = (float)xmlLayer->getDoubleAttribute("sustain", p.sustain);
          p.release = (float)xmlLayer->getDoubleAttribute("release", p.release);

          p.filterCutoff = (float)xmlLayer->getDoubleAttribute("filterCutoff",
                                                               p.filterCutoff);
          p.filterResonance = (float)xmlLayer->getDoubleAttribute(
              "filterResonance", p.filterResonance);

          p.lowNote = xmlLayer->getIntAttribute("lowNote", p.lowNote);
          p.highNote = xmlLayer->getIntAttribute("highNote", p.highNote);

          p.isMuted = xmlLayer->getBoolAttribute("isMuted", p.isMuted);
          p.isSoloed = xmlLayer->getBoolAttribute("isSoloed", p.isSoloed);

          // Restore Sample Path
          juce::String samplePath = xmlLayer->getStringAttribute("samplePath");
          if (samplePath.isNotEmpty()) {
            juce::File file(samplePath);
            if (file.existsAsFile()) {
              layer.loadSample(file);
            }
          } else {
            layer.clearSample();
          }
        }
      }
    }

    // 3. Restore MIDI Mappings
    if (auto *xmlMappings = xmlState->getChildByName("MidiMappings")) {
      juce::ScopedLock sl(midiLock);
      midiMappings.clear();
      for (auto *xmlMap : xmlMappings->getChildIterator()) {
        if (xmlMap->hasTagName("Map")) {
          juce::String paramId = xmlMap->getStringAttribute("paramId");
          int ccNumber = xmlMap->getIntAttribute("ccNumber", -1);
          if (paramId.isNotEmpty() && ccNumber >= 0) {
            midiMappings[paramId] = ccNumber;
          }
        }
      }
    }
  }
}

bool GranularSynthAudioProcessor::hasMidiMapping(
    const juce::String &paramId) const {
  juce::ScopedLock sl(midiLock);
  return midiMappings.find(paramId) != midiMappings.end();
}

int GranularSynthAudioProcessor::getMidiMappingCC(
    const juce::String &paramId) const {
  juce::ScopedLock sl(midiLock);
  auto it = midiMappings.find(paramId);
  if (it != midiMappings.end())
    return it->second;
  return -1;
}

void GranularSynthAudioProcessor::setMidiMapping(const juce::String &paramId,
                                                 int ccNumber) {
  juce::ScopedLock sl(midiLock);
  midiMappings[paramId] = ccNumber;
}

void GranularSynthAudioProcessor::removeMidiMapping(
    const juce::String &paramId) {
  juce::ScopedLock sl(midiLock);
  midiMappings.erase(paramId);
}

void GranularSynthAudioProcessor::clearAllMidiMappings() {
  juce::ScopedLock sl(midiLock);
  midiMappings.clear();
}

float GranularSynthAudioProcessor::getParameterValue(
    const juce::String &paramId) const {
  if (paramId.startsWith("layer/")) {
    auto tokens = juce::StringArray::fromTokens(paramId, "/", "");
    if (tokens.size() == 3) {
      int layerIndex = tokens[1].getIntValue();
      if (layerIndex >= 0 && layerIndex < 4) {
        auto *nonConstThis = const_cast<GranularSynthAudioProcessor *>(this);
        auto &p = nonConstThis->getLayer(layerIndex).getParams();
        auto name = tokens[2];
        if (name == "position")
          return p.position;
        if (name == "size")
          return p.size;
        if (name == "density")
          return p.density;
        if (name == "pitch")
          return p.pitch;
        if (name == "volume")
          return p.volume;
        if (name == "posRandomness")
          return p.posRandomness;
        if (name == "pitchRandomness")
          return p.pitchRandomness;
        if (name == "sizeRandomness")
          return p.sizeRandomness;
        if (name == "pan")
          return p.pan;
        if (name == "spread")
          return p.spread;
        if (name == "scanSpeed")
          return p.scanSpeed;
        if (name == "reverseProbability")
          return p.reverseProbability;
        if (name == "filterCutoff")
          return p.filterCutoff;
        if (name == "filterResonance")
          return p.filterResonance;
        if (name == "attack")
          return p.attack;
        if (name == "decay")
          return p.decay;
        if (name == "sustain")
          return p.sustain;
        if (name == "release")
          return p.release;
        if (name == "lowNote")
          return (float)p.lowNote;
        if (name == "highNote")
          return (float)p.highNote;
      }
    }
  } else if (paramId.startsWith("global/")) {
    auto name = paramId.substring(7);
    if (name == "reverbSize")
      return reverbParams.roomSize;
    if (name == "reverbWet")
      return reverbParams.wetLevel;
    if (name == "chorusRate")
      return fxParams.chorusRate;
    if (name == "chorusDepth")
      return fxParams.chorusDepth;
    if (name == "chorusMix")
      return fxParams.chorusMix;
    if (name == "delayTime")
      return fxParams.delayTime;
    if (name == "delayFeedback")
      return fxParams.delayFeedback;
    if (name == "delayMix")
      return fxParams.delayMix;
    if (name == "phaserRate")
      return fxParams.phaserRate;
    if (name == "phaserDepth")
      return fxParams.phaserDepth;
    if (name == "phaserCenterFreq")
      return fxParams.phaserCenterFreq;
    if (name == "phaserMix")
      return fxParams.phaserMix;
    if (name == "filterCutoff")
      return fxParams.filterCutoff;
    if (name == "filterResonance")
      return fxParams.filterResonance;
    if (name == "limiterThreshold")
      return fxParams.limiterThreshold;
    if (name == "limiterRelease")
      return fxParams.limiterRelease;
  }
  return 0.0f;
}

void GranularSynthAudioProcessor::setParameterValue(const juce::String &paramId,
                                                    float val) {
  if (paramId.startsWith("layer/")) {
    auto tokens = juce::StringArray::fromTokens(paramId, "/", "");
    if (tokens.size() == 3) {
      int layerIndex = tokens[1].getIntValue();
      if (layerIndex >= 0 && layerIndex < 4) {
        auto &p = getLayer(layerIndex).getParams();
        auto name = tokens[2];
        if (name == "position")
          p.position = val;
        else if (name == "size")
          p.size = val;
        else if (name == "density")
          p.density = val;
        else if (name == "pitch")
          p.pitch = val;
        else if (name == "volume")
          p.volume = val;
        else if (name == "posRandomness")
          p.posRandomness = val;
        else if (name == "pitchRandomness")
          p.pitchRandomness = val;
        else if (name == "sizeRandomness")
          p.sizeRandomness = val;
        else if (name == "pan")
          p.pan = val;
        else if (name == "spread")
          p.spread = val;
        else if (name == "scanSpeed")
          p.scanSpeed = val;
        else if (name == "reverseProbability")
          p.reverseProbability = val;
        else if (name == "filterCutoff")
          p.filterCutoff = val;
        else if (name == "filterResonance")
          p.filterResonance = val;
        else if (name == "attack")
          p.attack = val;
        else if (name == "decay")
          p.decay = val;
        else if (name == "sustain")
          p.sustain = val;
        else if (name == "release")
          p.release = val;
        else if (name == "lowNote")
          p.lowNote = (int)val;
        else if (name == "highNote")
          p.highNote = (int)val;
      }
    }
  } else if (paramId.startsWith("global/")) {
    auto name = paramId.substring(7);
    if (name == "reverbSize")
      reverbParams.roomSize = val;
    else if (name == "reverbWet")
      reverbParams.wetLevel = val;
    else if (name == "chorusRate")
      fxParams.chorusRate = val;
    else if (name == "chorusDepth")
      fxParams.chorusDepth = val;
    else if (name == "chorusMix")
      fxParams.chorusMix = val;
    else if (name == "delayTime")
      fxParams.delayTime = val;
    else if (name == "delayFeedback")
      fxParams.delayFeedback = val;
    else if (name == "delayMix")
      fxParams.delayMix = val;
    else if (name == "phaserRate")
      fxParams.phaserRate = val;
    else if (name == "phaserDepth")
      fxParams.phaserDepth = val;
    else if (name == "phaserCenterFreq")
      fxParams.phaserCenterFreq = val;
    else if (name == "phaserMix")
      fxParams.phaserMix = val;
    else if (name == "filterCutoff")
      fxParams.filterCutoff = val;
    else if (name == "filterResonance")
      fxParams.filterResonance = val;
    else if (name == "limiterThreshold")
      fxParams.limiterThreshold = val;
    else if (name == "limiterRelease")
      fxParams.limiterRelease = val;
  }
}

float GranularSynthAudioProcessor::mapCCValueToParam(
    const juce::String &paramId, float ccNormalized) {
  if (paramId.endsWith("/position") || paramId.endsWith("/posRandomness") ||
      paramId.endsWith("/pitchRandomness") ||
      paramId.endsWith("/sizeRandomness") || paramId.endsWith("/spread") ||
      paramId.endsWith("/reverseProbability") || paramId.endsWith("/sustain") ||
      paramId.endsWith("reverbSize") || paramId.endsWith("reverbWet") ||
      paramId.endsWith("chorusDepth") || paramId.endsWith("chorusMix")) {
    return ccNormalized;
  }
  if (paramId.endsWith("/volume")) {
    return ccNormalized;
  }
  if (paramId.endsWith("/size")) {
    return juce::jmap(ccNormalized, 0.01f, 1.0f);
  }
  if (paramId.endsWith("/density")) {
    return juce::jmap(ccNormalized, 1.0f, 100.0f);
  }
  if (paramId.endsWith("/pitch")) {
    return juce::jmap(ccNormalized, 0.1f, 4.0f);
  }
  if (paramId.endsWith("/pan")) {
    return juce::jmap(ccNormalized, -1.0f, 1.0f);
  }
  if (paramId.endsWith("/scanSpeed")) {
    return juce::jmap(ccNormalized, -2.0f, 2.0f);
  }
  if (paramId.endsWith("/filterCutoff")) {
    return 20.0f * std::pow(1000.0f, ccNormalized);
  }
  if (paramId.endsWith("/filterResonance") ||
      paramId.endsWith("filterResonance")) {
    return juce::jmap(ccNormalized, 0.1f, 2.0f);
  }
  if (paramId.endsWith("/attack") || paramId.endsWith("/decay")) {
    return juce::jmap(ccNormalized, 0.01f, 2.0f);
  }
  if (paramId.endsWith("/release")) {
    return juce::jmap(ccNormalized, 0.01f, 5.0f);
  }
  if (paramId.endsWith("/lowNote") || paramId.endsWith("/highNote")) {
    return juce::jmap(ccNormalized, 0.0f, 127.0f);
  }
  if (paramId.endsWith("chorusRate")) {
    return juce::jmap(ccNormalized, 0.1f, 10.0f);
  }
  if (paramId.endsWith("limiterThreshold")) {
    return juce::jmap(ccNormalized, -40.0f, 0.0f);
  }
  if (paramId.endsWith("limiterRelease")) {
    return juce::jmap(ccNormalized, 1.0f, 500.0f);
  }
  return ccNormalized;
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new GranularSynthAudioProcessor();
}
