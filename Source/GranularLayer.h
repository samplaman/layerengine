#pragma once

#include <JuceHeader.h>
#include <atomic>

enum class WindowShape { Sine, Gaussian, Hann, Triangular };

struct GranularParams {
  float position = 0.0f;
  float size = 0.1f;
  float density = 10.0f;
  float pitch = 1.0f;
  float volume = 0.8f;

  // Randomness / Jitter
  float posRandomness = 0.1f;
  float pitchRandomness = 0.0f;
  float sizeRandomness = 0.0f;

  // Stereo
  float pan = 0.0f;    // -1.0 to 1.0
  float spread = 0.5f; // 0.0 to 1.0

  // Advanced
  float scanSpeed = 0.0f;
  float reverseProbability = 0.0f;
  WindowShape windowShape = WindowShape::Sine;
  float pitchBend = 0.0f;

  // Envelope
  float attack = 0.1f;
  float decay = 0.1f;
  float sustain = 1.0f;
  float release = 0.5f;

  // Filter
  float filterCutoff = 20000.0f;
  float filterResonance = 0.7f;

  // Note Range
  int lowNote = 0;
  int highNote = 127;

  // Mute / Solo
  bool isMuted = false;
  bool isSoloed = false;
};

class GranularLayer {
public:
  GranularLayer();
  ~GranularLayer();

  void loadSample(const juce::File &file);
  void clearSample();
  void prepareToPlay(double sampleRate, int samplesPerBlock);
  void processBlock(juce::AudioBuffer<float> &buffer,
                    juce::MidiBuffer &midiMessages, float gain = 1.0f);

  GranularParams &getParams() { return params; }
  bool hasSample() const { return sampleBuffer.getNumSamples() > 0; }
  const juce::AudioSampleBuffer &getSampleBuffer() const {
    return sampleBuffer;
  }
  juce::String getCurrentSamplePath() const { return currentSamplePath; }
  float getCurrentLevel() const { return currentLevel.load(); }
  float getPlayhead() const { return playhead; }

  struct Grain {
    double startSample;
    double currentOffset;
    double length;
    float amplitude;
    float pitch;
    float panL;
    float panR;
    bool isReverse = false;
    bool active = false;
  };

  const std::vector<Grain> &getGrains() const { return grains; }

private:
  juce::AudioFormatManager formatManager;
  juce::AudioSampleBuffer sampleBuffer;
  juce::CriticalSection lock;

  GranularParams params;
  juce::String currentSamplePath;
  std::atomic<float> currentLevel{0.0f};
  float playhead = 0.0f;
  float lastPosParam = -1.0f;

  double currentSampleRate = 44100.0;
  std::vector<Grain> grains;
  int nextGrainCounter = 0;

  juce::ADSR adsr;
  std::set<int> activeNoteNumbers;
  int lastNoteNumber = 60;

  // Filter
  juce::dsp::StateVariableTPTFilter<float> filter;

  void spawnGrain();
  void handleMidi(juce::MidiBuffer &midiMessages);
};
