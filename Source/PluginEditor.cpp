#include "PluginEditor.h"
#include "PluginProcessor.h"

GranularSynthAudioProcessorEditor::GranularSynthAudioProcessorEditor(
    GranularSynthAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p),
      tabs(juce::TabbedButtonBar::TabsAtTop),
      keyboard(p.getKeyboardState(),
               juce::MidiKeyboardComponent::horizontalKeyboard) {
  setLookAndFeel(&customLookAndFeel);
  setSize(950, 700);
  setResizable(true, true);
  setResizeLimits(800, 500, 1920, 1080);

  for (int i = 0; i < 4; ++i) {
    auto *layerUI = new LayerUI(audioProcessor.getLayer(i));
    tabs.addTab("LAYER ENGINE " + juce::String(i + 1),
                juce::Colours::transparentBlack, layerUI, true);
  }

  auto *mixerUI = new MixerUI(audioProcessor);
  tabs.addTab("MIXER", juce::Colours::transparentBlack, mixerUI, true);

  auto *fxUI = new EffectsUI(audioProcessor);
  tabs.addTab("FX", juce::Colours::transparentBlack, fxUI, true);

  addAndMakeVisible(tabs);
  addAndMakeVisible(keyboard);

  keyboard.setKeyWidth(12);
  keyboard.setScrollButtonsVisible(true);
  keyboard.setLowestVisibleKey(48);

  // Pitch & Mod Wheels
  addAndMakeVisible(pitchWheel);
  pitchWheel.setSliderStyle(juce::Slider::LinearVertical);
  pitchWheel.setRange(-1.0, 1.0);
  pitchWheel.setValue(0.0);
  pitchWheel.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
  pitchWheel.onValueChange = [this] {
    for (int i = 0; i < 4; ++i)
      audioProcessor.getLayer(i).getParams().pitchBend =
          (float)pitchWheel.getValue();
  };

  addAndMakeVisible(modWheel);
  modWheel.setSliderStyle(juce::Slider::LinearVertical);
  modWheel.setRange(0.0, 1.0);
  modWheel.setValue(0.0);
  modWheel.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
  modWheel.onValueChange = [this] {
    for (int i = 0; i < 4; ++i)
      audioProcessor.getLayer(i).getParams().position =
          (float)modWheel.getValue();
  };

  pitchLabel.setText("PITCH", juce::dontSendNotification);
  pitchLabel.setFont(juce::FontOptions(10.0f).withStyle("Bold"));
  pitchLabel.setJustificationType(juce::Justification::centred);
  addAndMakeVisible(pitchLabel);

  modLabel.setText("MOD", juce::dontSendNotification);
  modLabel.setFont(juce::FontOptions(10.0f).withStyle("Bold"));
  modLabel.setJustificationType(juce::Justification::centred);
  addAndMakeVisible(modLabel);

  tabs.setIndent(15);

  logoImage = juce::ImageFileFormat::loadFrom(BinaryData::logo_png,
                                              BinaryData::logo_pngSize);
  backgroundImage = juce::ImageFileFormat::loadFrom(
      BinaryData::mountains_png, BinaryData::mountains_pngSize);
}

GranularSynthAudioProcessorEditor::~GranularSynthAudioProcessorEditor() {
  setLookAndFeel(nullptr);
}

void GranularSynthAudioProcessorEditor::paint(juce::Graphics &g) {
  if (backgroundImage.isValid()) {
    g.drawImageWithin(backgroundImage, 0, 0, getWidth(), getHeight(),
                      juce::RectanglePlacement::fillDestination);
    g.fillAll(juce::Colours::black.withAlpha(
        0.2f)); // Lighter overlay for blurred background
  } else {
    g.fillAll(juce::Colour(0xff131212)); // HTML #131212ff
  }

  // Header
  auto headerArea = getLocalBounds().removeFromTop(70).reduced(20, 15);
  if (logoImage.isValid()) {
    auto logoBounds = headerArea.removeFromLeft(180);
    g.drawImageWithin(logoImage, logoBounds.getX(), logoBounds.getY(),
                      logoBounds.getWidth(), logoBounds.getHeight(),
                      juce::RectanglePlacement::xLeft |
                          juce::RectanglePlacement::yMid |
                          juce::RectanglePlacement::onlyReduceInSize);
  } else {
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(32.0f).withStyle("Bold"));
    g.drawText("LAYER ENGINE", headerArea.removeFromLeft(200).getX(),
               headerArea.getY(), 200, headerArea.getHeight(),
               juce::Justification::left);
  }

  headerArea.removeFromLeft(20); // Gap
  g.setColour(juce::Colours::white.withAlpha(0.8f));
  g.setFont(juce::FontOptions(15.0f));
  g.drawText("", headerArea.getX(), headerArea.getY(), 300,
             headerArea.getHeight(), juce::Justification::left);

  // Keyboard & Wheels Transparent Container
  auto bottomArea = getLocalBounds().removeFromBottom(100).reduced(15, 5);
}

void GranularSynthAudioProcessorEditor::resized() {
  auto area = getLocalBounds();
  area.removeFromTop(70);

  auto bottomArea = area.removeFromBottom(100).reduced(25, 5);

  auto wheelArea = bottomArea.removeFromLeft(80);
  auto pitchArea = wheelArea.removeFromLeft(35);
  pitchLabel.setBounds(pitchArea.removeFromBottom(15));
  pitchWheel.setBounds(pitchArea.reduced(5, 2));

  auto modArea = wheelArea.removeFromLeft(35);
  modLabel.setBounds(modArea.removeFromBottom(15));
  modWheel.setBounds(modArea.reduced(5, 2));

  keyboard.setBounds(bottomArea.reduced(5, 5));
  keyboard.setKeyWidth(bottomArea.getWidth() / 60.0f);
  keyboard.setLowestVisibleKey(24); // Show more range, starting from C1

  tabs.setBounds(area.reduced(15, 0));
}
