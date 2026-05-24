#pragma once

#include "PluginProcessor.h"
#include <JuceHeader.h>

// Premium Light Glassmorphism Theme
class CustomLookAndFeel : public juce::LookAndFeel_V4 {
public:
  CustomLookAndFeel() {
    auto accentColor = juce::Colour(9, 136, 131); // Rich Teal
    auto darkText = juce::Colour(0xff2d3436);

    setColour(juce::Slider::thumbColourId, accentColor);
    setColour(juce::Slider::rotarySliderOutlineColourId,
              juce::Colours::white.withAlpha(0.5f));
    setColour(juce::Slider::rotarySliderFillColourId, accentColor);
    setColour(juce::Slider::trackColourId,
              juce::Colours::white.withAlpha(0.5f));

    setColour(juce::TextButton::buttonColourId,
              juce::Colours::white.withAlpha(0.6f));
    setColour(juce::TextButton::buttonOnColourId, accentColor.withAlpha(0.2f));
    setColour(juce::TextButton::textColourOffId, darkText);
    setColour(juce::TextButton::textColourOnId, darkText);

    setColour(juce::TabbedButtonBar::tabTextColourId, darkText);
    setColour(juce::TabbedButtonBar::tabOutlineColourId,
              juce::Colours::transparentBlack);
    setColour(juce::TabbedComponent::outlineColourId,
              juce::Colours::transparentBlack);

    setColour(juce::Label::textColourId, darkText);

    // Keyboard Styling
    setColour(juce::MidiKeyboardComponent::whiteNoteColourId,
              juce::Colours::white.withAlpha(0.95f));
    setColour(juce::MidiKeyboardComponent::blackNoteColourId,
              juce::Colour(0xff222222));
    setColour(juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId,
              accentColor.withAlpha(0.2f));
    setColour(juce::MidiKeyboardComponent::keyDownOverlayColourId,
              accentColor.withAlpha(0.4f));
  }

  void drawTabButton(juce::TabBarButton &button, juce::Graphics &g,
                     bool isMouseOver, bool isMouseDown) override {
    auto area = button.getLocalBounds().toFloat().reduced(2, 2);
    bool isSelected = button.isFrontTab();

    if (isSelected || isMouseOver) {
      g.setColour(juce::Colours::white.withAlpha(isSelected ? 0.9f : 0.2f));
      g.fillRoundedRectangle(area, 6.0f);

      g.setColour(juce::Colours::white.withAlpha(isSelected ? 1.0f : 0.4f));
      g.drawRoundedRectangle(area, 6.0f, 1.0f);
    }

    g.setColour(isSelected ? juce::Colour(9, 136, 131)
                           : juce::Colours::white.withAlpha(0.85f));
    g.setFont(
        juce::FontOptions(13.0f).withStyle(isSelected ? "Bold" : "Plain"));
    g.drawText(button.getButtonText(), area, juce::Justification::centred);
  }

  void drawTabbedButtonBarBackground(juce::TabbedButtonBar &,
                                     juce::Graphics &g) override {
    // Keep it transparent
  }

  void drawTabAreaBehindFrontButton(juce::TabbedButtonBar &, juce::Graphics &,
                                    int, int) override {
    // Remove shadow under tab header
  }

  void drawButtonBackground(juce::Graphics &g, juce::Button &button,
                            const juce::Colour &backgroundColour,
                            bool shouldDrawButtonAsHighlighted,
                            bool shouldDrawButtonAsDown) override {
    if (backgroundColour == juce::Colours::transparentBlack) {
      if (shouldDrawButtonAsHighlighted || shouldDrawButtonAsDown) {
        auto area = button.getLocalBounds().toFloat();
        g.setColour(juce::Colours::white.withAlpha(
            shouldDrawButtonAsDown ? 0.2f : 0.1f));
        g.fillRoundedRectangle(area, 6.0f);
      }
      return;
    }
    juce::LookAndFeel_V4::drawButtonBackground(g, button, backgroundColour,
                                               shouldDrawButtonAsHighlighted,
                                               shouldDrawButtonAsDown);
  }

  juce::Font getTextButtonFont(juce::TextButton &button,
                               int buttonHeight) override {
    if (button.getButtonText() == juce::CharPointer_UTF8("\xe2\x93\x98"))
      return juce::FontOptions(22.0f);
    return juce::LookAndFeel_V4::getTextButtonFont(button, buttonHeight);
  }

  void drawComboBox(juce::Graphics &g, int width, int height, bool isButtonDown,
                    int buttonX, int buttonY, int buttonW, int buttonH,
                    juce::ComboBox &box) override {
    auto area = juce::Rectangle<int>(0, 0, width, height).toFloat().reduced(2);

    g.setColour(juce::Colours::white.withAlpha(0.6f));
    g.fillRoundedRectangle(area, 4.0f);

    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(area, 4.0f, 1.0f);

    // Draw the little arrow
    juce::Path p;
    float arrowSize = 0.25f * (float)juce::jmin(buttonW, buttonH);
    p.addTriangle((float)buttonX + (float)buttonW * 0.5f - arrowSize,
                  (float)buttonY + (float)buttonH * 0.5f - arrowSize * 0.5f,
                  (float)buttonX + (float)buttonW * 0.5f + arrowSize,
                  (float)buttonY + (float)buttonH * 0.5f - arrowSize * 0.5f,
                  (float)buttonX + (float)buttonW * 0.5f,
                  (float)buttonY + (float)buttonH * 0.5f + arrowSize * 0.5f);

    g.setColour(juce::Colours::white.withAlpha(0.6f));
    g.fillPath(p);
  }

  void drawPopupMenuBackground(juce::Graphics &g, int width,
                               int height) override {
    auto area = juce::Rectangle<int>(0, 0, width, height).toFloat();
    juce::ColourGradient bgGrad(
        juce::Colours::white.withAlpha(0.9f), area.getTopLeft(),
        juce::Colours::white.withAlpha(0.7f), area.getBottomRight(), false);
    g.setGradientFill(bgGrad);
    g.fillRoundedRectangle(area, 6.0f);

    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(area.reduced(0.5f), 6.0f, 1.5f);
  }

  void drawAlertBox(juce::Graphics &g, juce::AlertWindow &alert,
                    const juce::Rectangle<int> &textArea,
                    juce::TextLayout &textLayout) override {
    auto area = alert.getLocalBounds().toFloat();

    juce::ColourGradient bgGrad(
        juce::Colours::white.withAlpha(0.95f), area.getTopLeft(),
        juce::Colours::white.withAlpha(0.85f), area.getBottomRight(), false);
    g.setGradientFill(bgGrad);

    // Fill the window completely since AlertWindows might not support
    // transparency at the OS level
    g.fillAll(juce::Colours::white.withAlpha(0.95f));

    g.setGradientFill(bgGrad);
    g.fillRect(area);

    g.setColour(juce::Colours::white);
    g.drawRect(area, 2.0f);

    g.setColour(juce::Colour(0xff2d3436));
    textLayout.draw(g, textArea.toFloat());
  }

  void drawPopupMenuItem(juce::Graphics &g, const juce::Rectangle<int> &area,
                         bool isSeparator, bool isActive, bool isHighlighted,
                         bool isChecked, bool isEnabled,
                         const juce::String &text,
                         const juce::String &shortcutKeyText,
                         const juce::Drawable *icon,
                         const juce::Colour *textColourToUse) override {
    if (isHighlighted && isEnabled) {
      g.setColour(juce::Colours::black.withAlpha(0.08f));
      g.fillRect(area.reduced(2));
    }

    g.setColour(isEnabled ? juce::Colour(0xff2d3436)
                          : juce::Colour(0xff2d3436).withAlpha(0.4f));
    g.setFont(juce::FontOptions(13.0f));

    auto r = area.reduced(10, 0);
    g.drawText(text, r, juce::Justification::centredLeft, true);
  }

  void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height,
                        float sliderPos, const float rotaryStartAngle,
                        const float rotaryEndAngle,
                        juce::Slider &slider) override {
    auto bounds =
        juce::Rectangle<int>(x, y, width, height).toFloat().reduced(10);
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle =
        rotaryStartAngle +
        sliderPos * (rotaryStartAngle - rotaryEndAngle); // Wait, fix direction?
    // Actually toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle -
    // rotaryStartAngle);
    toAngle =
        rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = 4.5f; // Chunky!
    auto arcRadius = radius - lineW * 0.5f;

    // Background Arc
    juce::Path backgroundArc;
    backgroundArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(),
                                arcRadius, arcRadius, 0.0f, rotaryStartAngle,
                                rotaryEndAngle, true);
    g.setColour(juce::Colours::white.withAlpha(0.2f));
    g.strokePath(backgroundArc,
                 juce::PathStrokeType(lineW, juce::PathStrokeType::curved,
                                      juce::PathStrokeType::rounded));

    if (slider.isEnabled()) {
      // Solid White Value Arc
      juce::Path valueArc;
      valueArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(),
                             arcRadius, arcRadius, 0.0f, rotaryStartAngle,
                             toAngle, true);
      g.setColour(juce::Colours::white);
      g.strokePath(valueArc,
                   juce::PathStrokeType(lineW, juce::PathStrokeType::curved,
                                        juce::PathStrokeType::rounded));

      // Chunky Pointer
      g.setColour(juce::Colours::white);
      auto pointerLength = radius * 0.7f;
      juce::Path needle;
      needle.startNewSubPath(bounds.getCentreX(), bounds.getCentreY());
      needle.lineTo(
          bounds.getCentreX() +
              pointerLength *
                  std::cos(toAngle - juce::MathConstants<float>::halfPi),
          bounds.getCentreY() +
              pointerLength *
                  std::sin(toAngle - juce::MathConstants<float>::halfPi));
      g.strokePath(needle,
                   juce::PathStrokeType(lineW, juce::PathStrokeType::curved,
                                        juce::PathStrokeType::rounded));
    }
  }

  void drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height,
                        float sliderPos, float minSliderPos, float maxSliderPos,
                        const juce::Slider::SliderStyle style,
                        juce::Slider &slider) override {
    if (style == juce::Slider::LinearHorizontal ||
        style == juce::Slider::LinearBar ||
        style == juce::Slider::LinearBarVertical) {
      juce::LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos,
                                             minSliderPos, maxSliderPos, style,
                                             slider);
      return;
    }

    // Custom Linear Vertical Style (White)
    auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat();
    float trackWidth = 4.0f;
    juce::Rectangle<float> track(bounds.getCentreX() - trackWidth * 0.5f,
                                 bounds.getY() + 10.0f, trackWidth,
                                 bounds.getHeight() - 20.0f);

    g.setColour(juce::Colours::white.withAlpha(0.2f));
    g.fillRoundedRectangle(track, trackWidth * 0.5f);

    if (slider.isEnabled()) {
      // Filled track from thumb to bottom
      juce::Rectangle<float> valTrack(track.getX(), sliderPos, track.getWidth(),
                                      track.getBottom() - sliderPos);

      // If it's bipolar (e.g. pan), we should draw from center, but for
      // simplicity we draw from thumb down or just use the slider value Wait,
      // pan is Rotary. Pitch wheel and Mod wheel are vertical. Mod wheel is 0
      // to 1, Pitch is -1 to 1. ADSR sliders are 0 to 1. For pitch wheel,
      // sliderPos could be anywhere. Let's just draw from thumb to zero pos (if
      // we can find it), or simply don't fill the track at all, just a clean
      // white thumb on a dark track! Actually, drawing the value track for
      // vertical sliders looks great if they are 0-based. Let's just draw the
      // value track from thumb to bottom.
      g.setColour(juce::Colours::white.withAlpha(0.8f));
      g.fillRoundedRectangle(valTrack, trackWidth * 0.5f);

      // Thumb
      float thumbSize = 16.0f;
      juce::Rectangle<float> thumb(bounds.getCentreX() - thumbSize * 0.5f,
                                   sliderPos - thumbSize * 0.5f, thumbSize,
                                   thumbSize);

      g.setColour(juce::Colours::white);
      g.fillEllipse(thumb);

      // Thumb glow
      g.setColour(juce::Colours::white.withAlpha(0.3f));
      g.drawEllipse(thumb.reduced(-1.0f), 1.5f);
    }
  }
};

class CustomKeyboard : public juce::MidiKeyboardComponent {
public:
  CustomKeyboard(juce::MidiKeyboardState &state,
                 juce::MidiKeyboardComponent::Orientation orientation)
      : juce::MidiKeyboardComponent(state, orientation) {}

  std::function<std::pair<int, int>()> getActiveNoteRange;

  void drawWhiteNote(int midiNoteNumber, juce::Graphics &g,
                     juce::Rectangle<float> area, bool isDown, bool isOver,
                     juce::Colour lineColour,
                     juce::Colour textColour) override {
    auto accent = juce::Colour(9, 136, 131);

    bool inRange = true;
    if (getActiveNoteRange) {
      auto range = getActiveNoteRange();
      inRange = (midiNoteNumber >= range.first && midiNoteNumber <= range.second);
    }

    juce::Colour fillColour =
        isDown ? accent.withAlpha(0.7f)
               : (isOver ? juce::Colours::white.withAlpha(0.45f)
                         : juce::Colours::white.withAlpha(0.25f));

    if (!inRange) {
      fillColour = fillColour.withMultipliedAlpha(0.3f);
    }

    auto keyArea = area.reduced(0.5f, 0.0f);

    // Flat key body
    g.setColour(fillColour);
    g.fillRect(keyArea);

    // Thin solid border
    g.setColour(juce::Colours::black.withAlpha(0.15f));
    g.drawRect(keyArea, 1.0f);

    // Draw C note labels
    if (midiNoteNumber % 12 == 0) {
      g.setColour(juce::Colour(0xff2d3436).withAlpha(isDown ? 0.9f : 0.6f));
      g.setFont(juce::FontOptions(11.0f).withStyle("Bold"));
      g.drawText("C" + juce::String(midiNoteNumber / 12 - 2),
                 keyArea.withTrimmedBottom(4.0f),
                 juce::Justification::centredBottom, false);
    }
  }

  void drawBlackNote(int midiNoteNumber, juce::Graphics &g,
                     juce::Rectangle<float> area, bool isDown, bool isOver,
                     juce::Colour noteFillColour) override {
    auto accent = juce::Colour(9, 136, 131);

    bool inRange = true;
    if (getActiveNoteRange) {
      auto range = getActiveNoteRange();
      inRange = (midiNoteNumber >= range.first && midiNoteNumber <= range.second);
    }

    juce::Colour fillColour =
        isDown ? accent.withAlpha(0.8f)
               : (isOver ? juce::Colours::black.withAlpha(0.7f)
                         : juce::Colours::black.withAlpha(0.4f));

    if (!inRange) {
      fillColour = fillColour.withMultipliedAlpha(0.3f);
    }

    auto keyArea =
        area.reduced(1.5f, 0.0f).withTrimmedBottom(isDown ? 1.0f : 2.0f);

    // Solid flat fill
    g.setColour(fillColour);
    g.fillRect(keyArea);

    // Thin solid border
    g.setColour(juce::Colours::black.withAlpha(0.4f));
    g.drawRect(keyArea, 1.0f);
  }
};

class SpringSlider : public juce::Slider {
public:
  SpringSlider() : juce::Slider() {}
  void mouseUp(const juce::MouseEvent &e) override {
    juce::Slider::mouseUp(e);
    setValue(0.0, juce::sendNotificationAsync);
  }
};

class WaveformViewer : public juce::Component {
public:
  WaveformViewer(GranularLayer &l) : layer(l) {}

  void paint(juce::Graphics &g) override {
    auto bounds = getLocalBounds().toFloat();
    // Transparent background

    auto &buffer = layer.getSampleBuffer();
    if (buffer.getNumSamples() > 0) {
      g.setColour(juce::Colour(0xff2d3436).withAlpha(0.5f)); // Darker waveform
      auto ratio = (float)buffer.getNumSamples() / (float)getWidth();

      juce::Path p;
      p.startNewSubPath(0, getHeight() / 2.0f);

      for (int x = 0; x < getWidth(); ++x) {
        int sampleIdx = (int)(x * ratio);
        float val = buffer.getSample(0, sampleIdx);
        p.lineTo((float)x, (getHeight() / 2.0f) + (val * getHeight() / 2.2f));
      }
      g.strokePath(p, juce::PathStrokeType(1.5f));

      for (const auto &grain : layer.getGrains()) {
        if (grain.active) {
          float xCurrent = (float)(grain.startSample + grain.currentOffset) /
                           buffer.getNumSamples() * getWidth();
          float alpha = std::sin(((float)grain.currentOffset / grain.length) *
                                 juce::MathConstants<float>::pi);
          g.setColour(juce::Colour(9, 136, 131)
                          .withAlpha(alpha * 0.7f)); // Vibrant pink grains
          g.drawVerticalLine((int)xCurrent, 5.0f, (float)getHeight() - 5.0f);
        }
      }

      g.setColour(juce::Colour(0xff2d3436).withAlpha(0.4f));
      float posIdx = layer.getPlayhead() * getWidth();
      g.drawVerticalLine((int)posIdx, 2.0f, (float)getHeight() - 2.0f);
    }
  }

private:
  GranularLayer &layer;
};

class EnvelopeVisualizer : public juce::Component, public juce::Timer {
public:
  EnvelopeVisualizer(GranularLayer &l) : layer(l) { startTimerHz(30); }

  void timerCallback() override { repaint(); }

  void paint(juce::Graphics &g) override {
    auto bounds = getLocalBounds().toFloat();

    auto &p = layer.getParams();

    // Map ADSR to x/y coordinates
    float totalTime = p.attack + p.decay + p.release + 1.0f; // 1.0s for sustain phase visual width
    if (totalTime < 0.1f)
      totalTime = 0.1f;

    float w = bounds.getWidth() - 30.0f;
    float h = bounds.getHeight() - 30.0f;
    float startX = 15.0f;
    float bottomY = bounds.getBottom() - 15.0f;
    float topY = bounds.getY() + 15.0f;

    float aX = startX + (p.attack / totalTime) * w;
    float dX = aX + (p.decay / totalTime) * w;
    float sX = dX + (1.0f / totalTime) * w; // Fixed visual width for sustain
    float rX = sX + (p.release / totalTime) * w;

    float sY = bottomY - p.sustain * h;

    juce::Path envPath;
    envPath.startNewSubPath(startX, bottomY); // Start
    envPath.lineTo(aX, topY);                 // Attack
    envPath.lineTo(dX, sY);                   // Decay
    envPath.lineTo(sX, sY);                   // Sustain
    envPath.lineTo(rX, bottomY);              // Release

    // Fill the path
    juce::Path filledPath = envPath;
    filledPath.lineTo(startX, bottomY);
    filledPath.closeSubPath();

    // Gradient fill
    juce::Colour accent(9, 136, 131);
    juce::ColourGradient grad(accent.withAlpha(0.6f), 0, topY,
                              accent.withAlpha(0.0f), 0, bottomY, false);
    g.setGradientFill(grad);
    g.fillPath(filledPath);

    // Stroke
    g.setColour(accent.brighter(0.2f));
    g.strokePath(envPath, juce::PathStrokeType(2.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Draw small circles at nodes
    g.setColour(juce::Colours::white);
    float radius = 3.5f;
    g.fillEllipse(startX - radius, bottomY - radius, radius * 2, radius * 2);
    g.fillEllipse(aX - radius, topY - radius, radius * 2, radius * 2);
    g.fillEllipse(dX - radius, sY - radius, radius * 2, radius * 2);
    g.fillEllipse(sX - radius, sY - radius, radius * 2, radius * 2);
    g.fillEllipse(rX - radius, bottomY - radius, radius * 2, radius * 2);

    // Label
    g.setColour(juce::Colours::white.withAlpha(0.6f));
    g.setFont(juce::FontOptions(11.0f).withStyle("Bold"));
    g.drawText("ADSR", bounds.withTrimmedTop(8).withTrimmedRight(10),
               juce::Justification::topRight);
  }

private:
  GranularLayer &layer;
};

class MappedSlider : public juce::Slider {
public:
  MappedSlider() : audioProcessor(nullptr) {}

  void init(const juce::String &parameterId,
            GranularSynthAudioProcessor &processor) {
    paramId = parameterId;
    audioProcessor = &processor;
  }

  void mouseDown(const juce::MouseEvent &e) override {
    if (audioProcessor != nullptr && e.mods.isRightButtonDown()) {
      showPopupMenu();
    } else {
      juce::Slider::mouseDown(e);
    }
  }

  void paint(juce::Graphics &g) override {
    juce::Slider::paint(g);

    if (audioProcessor == nullptr)
      return;

    bool isLearning = (audioProcessor->getLearningParamID() == paramId);
    bool hasMapping = audioProcessor->hasMidiMapping(paramId);

    if (isLearning) {
      auto bounds = getLocalBounds().toFloat();
      g.setColour(juce::Colour(220, 80, 80).withAlpha(0.2f));
      g.fillRoundedRectangle(bounds, 6.0f);

      g.setColour(juce::Colour(220, 80, 80).withAlpha(0.7f));
      g.drawRoundedRectangle(bounds, 6.0f, 1.5f);

      g.setFont(juce::FontOptions(10.0f).withStyle("Bold"));
      g.setColour(juce::Colours::white);
      g.drawText("LEARN", bounds, juce::Justification::centred);
    } else if (hasMapping) {
      auto bounds = getLocalBounds();
      g.setColour(juce::Colour(9, 136, 131)); // Glowing Teal mapping dot
      g.fillEllipse((float)(bounds.getWidth() - 9), 3.0f, 5.0f, 5.0f);
      g.setColour(juce::Colours::white.withAlpha(0.6f));
      g.drawEllipse((float)(bounds.getWidth() - 9), 3.0f, 5.0f, 5.0f, 0.5f);
    }
  }

private:
  void showPopupMenu() {
    if (audioProcessor == nullptr)
      return;

    juce::PopupMenu menu;

    bool isLearning = (audioProcessor->getLearningParamID() == paramId);
    bool hasMapping = audioProcessor->hasMidiMapping(paramId);
    int currentCC = audioProcessor->getMidiMappingCC(paramId);

    if (isLearning) {
      menu.addItem(1, "Learning MIDI CC... (Move controller now)", false);
    } else {
      if (hasMapping)
        menu.addItem(1, "MIDI Learned: CC " + juce::String(currentCC));
      else
        menu.addItem(1, "MIDI Learn");
    }

    menu.addItem(2, "Remove MIDI Mapping", hasMapping);

    menu.showMenuAsync(juce::PopupMenu::Options(), [this](int result) {
      if (audioProcessor == nullptr)
        return;

      if (result == 1) {
        audioProcessor->setLearningParamID(paramId);
      } else if (result == 2) {
        audioProcessor->removeMidiMapping(paramId);
      }
    });
  }

  juce::String paramId;
  GranularSynthAudioProcessor *audioProcessor;
};

class LayerUI : public juce::Component,
                public juce::Timer,
                public juce::FileDragAndDropTarget {
public:
  LayerUI(GranularSynthAudioProcessor &p, int layerIdx)
      : audioProcessor(p), layerIndex(layerIdx), layer(p.getLayer(layerIdx)),
        viewer(p.getLayer(layerIdx)), envVisualizer(p.getLayer(layerIdx)) {

    posSlider.init("layer/" + juce::String(layerIndex) + "/position",
                   audioProcessor);
    sizeSlider.init("layer/" + juce::String(layerIndex) + "/size",
                    audioProcessor);
    densSlider.init("layer/" + juce::String(layerIndex) + "/density",
                    audioProcessor);
    pitchSlider.init("layer/" + juce::String(layerIndex) + "/pitch",
                     audioProcessor);

    posRandSlider.init("layer/" + juce::String(layerIndex) + "/posRandomness",
                       audioProcessor);
    pitchRandSlider.init("layer/" + juce::String(layerIndex) +
                             "/pitchRandomness",
                         audioProcessor);
    sizeRandSlider.init("layer/" + juce::String(layerIndex) + "/sizeRandomness",
                        audioProcessor);

    panSlider.init("layer/" + juce::String(layerIndex) + "/pan",
                   audioProcessor);
    spreadSlider.init("layer/" + juce::String(layerIndex) + "/spread",
                      audioProcessor);

    scanSpeedSlider.init("layer/" + juce::String(layerIndex) + "/scanSpeed",
                         audioProcessor);
    revProbSlider.init("layer/" + juce::String(layerIndex) +
                           "/reverseProbability",
                       audioProcessor);

    cutoffSlider.init("layer/" + juce::String(layerIndex) + "/filterCutoff",
                      audioProcessor);
    resSlider.init("layer/" + juce::String(layerIndex) + "/filterResonance",
                   audioProcessor);

    lowNoteSlider.init("layer/" + juce::String(layerIndex) + "/lowNote", audioProcessor);
    highNoteSlider.init("layer/" + juce::String(layerIndex) + "/highNote", audioProcessor);

    atkSlider.init("layer/" + juce::String(layerIndex) + "/attack",
                   audioProcessor);
    decSlider.init("layer/" + juce::String(layerIndex) + "/decay",
                   audioProcessor);
    susSlider.init("layer/" + juce::String(layerIndex) + "/sustain",
                   audioProcessor);
    relSlider.init("layer/" + juce::String(layerIndex) + "/release",
                   audioProcessor);

    addAndMakeVisible(viewer);
    addAndMakeVisible(envVisualizer);

    addAndMakeVisible(loadButton);
    loadButton.setButtonText("IMPORT AUDIO");
    loadButton.onClick = [this] {
      chooser = std::make_unique<juce::FileChooser>(
          "Select a sample",
          juce::File::getSpecialLocation(juce::File::userHomeDirectory),
          "*.wav;*.aif;*.mp3");
      chooser->launchAsync(juce::FileBrowserComponent::openMode |
                               juce::FileBrowserComponent::canSelectFiles,
                           [this](const juce::FileChooser &fc) {
                             auto file = fc.getResult();
                             if (file.existsAsFile()) {
                               this->layer.loadSample(file);
                               sampleNameLabel.setText(
                                   file.getFileName().toUpperCase(),
                                   juce::dontSendNotification);
                             }
                           });
    };

    addAndMakeVisible(clearButton);
    clearButton.setButtonText("CLEAR");
    clearButton.onClick = [this] {
      this->layer.clearSample();
      sampleNameLabel.setText("READY FOR SAMPLE", juce::dontSendNotification);
    };

    addAndMakeVisible(sampleNameLabel);
    sampleNameLabel.setText("READY FOR SAMPLE", juce::dontSendNotification);
    sampleNameLabel.setFont(juce::FontOptions(14.0f).withStyle("Bold"));
    sampleNameLabel.setColour(juce::Label::textColourId,
                              juce::Colour(0xff2d3436));

    GranularParams dp;

    setupSlider(posSlider, "POS", 0.0f, 1.0f, layer.getParams().position,
                dp.position, true);
    setupSlider(sizeSlider, "SIZE", 0.01f, 1.0f, layer.getParams().size,
                dp.size, true);
    setupSlider(densSlider, "DENS", 1.0f, 100.0f, layer.getParams().density,
                dp.density, true);
    setupSlider(pitchSlider, "PITCH", 0.1f, 4.0f, layer.getParams().pitch,
                dp.pitch, true);

    setupSlider(posRandSlider, "POS JIT", 0.0f, 1.0f,
                layer.getParams().posRandomness, dp.posRandomness, true);
    setupSlider(pitchRandSlider, "PITCH JIT", 0.0f, 1.0f,
                layer.getParams().pitchRandomness, dp.pitchRandomness, true);
    setupSlider(sizeRandSlider, "SIZE JIT", 0.0f, 1.0f,
                layer.getParams().sizeRandomness, dp.sizeRandomness, true);

    setupSlider(panSlider, "PAN", -1.0f, 1.0f, layer.getParams().pan, dp.pan,
                true);
    setupSlider(spreadSlider, "SPREAD", 0.0f, 1.0f, layer.getParams().spread,
                dp.spread, true);

    setupSlider(scanSpeedSlider, "SCAN", -2.0f, 2.0f,
                layer.getParams().scanSpeed, dp.scanSpeed, true);
    setupSlider(revProbSlider, "REV %", 0.0f, 1.0f,
                layer.getParams().reverseProbability, dp.reverseProbability,
                true);

    addAndMakeVisible(windowShapeCombo);
    windowShapeCombo.addItem("SINE", 1);
    windowShapeCombo.addItem("HANN", 2);
    windowShapeCombo.addItem("GAUSS", 3);
    windowShapeCombo.addItem("TRI", 4);
    windowShapeCombo.setSelectedId((int)layer.getParams().windowShape + 1);

    setupSlider(cutoffSlider, "FREQ", 20.0f, 20000.0f,
                layer.getParams().filterCutoff, dp.filterCutoff, true);
    cutoffSlider.setSkewFactorFromMidPoint(1000.0f);
    setupSlider(resSlider, "RES", 0.1f, 1.0f, layer.getParams().filterResonance,
                dp.filterResonance, true);

    setupSlider(lowNoteSlider, "KEY LO", 0.0f, 127.0f, layer.getParams().lowNote,
                0.0f, true);
    setupSlider(highNoteSlider, "KEY HI", 0.0f, 127.0f, layer.getParams().highNote,
                127.0f, true);

    setupSlider(atkSlider, "A", 0.01f, 2.0f, layer.getParams().attack,
                dp.attack);
    setupSlider(decSlider, "D", 0.01f, 2.0f, layer.getParams().decay, dp.decay);
    setupSlider(susSlider, "S", 0.0f, 1.0f, layer.getParams().sustain,
                dp.sustain);
    setupSlider(relSlider, "R", 0.01f, 5.0f, layer.getParams().release,
                dp.release);

    posSlider.onValueChange = [this] {
      this->layer.getParams().position = (float)posSlider.getValue();
    };
    sizeSlider.onValueChange = [this] {
      this->layer.getParams().size = (float)sizeSlider.getValue();
    };
    densSlider.onValueChange = [this] {
      this->layer.getParams().density = (float)densSlider.getValue();
    };
    pitchSlider.onValueChange = [this] {
      this->layer.getParams().pitch = (float)pitchSlider.getValue();
    };

    posRandSlider.onValueChange = [this] {
      this->layer.getParams().posRandomness = (float)posRandSlider.getValue();
    };
    pitchRandSlider.onValueChange = [this] {
      this->layer.getParams().pitchRandomness =
          (float)pitchRandSlider.getValue();
    };
    sizeRandSlider.onValueChange = [this] {
      this->layer.getParams().sizeRandomness = (float)sizeRandSlider.getValue();
    };

    panSlider.onValueChange = [this] {
      this->layer.getParams().pan = (float)panSlider.getValue();
    };
    spreadSlider.onValueChange = [this] {
      this->layer.getParams().spread = (float)spreadSlider.getValue();
    };

    scanSpeedSlider.onValueChange = [this] {
      this->layer.getParams().scanSpeed = (float)scanSpeedSlider.getValue();
    };
    revProbSlider.onValueChange = [this] {
      this->layer.getParams().reverseProbability =
          (float)revProbSlider.getValue();
    };

    windowShapeCombo.onChange = [this] {
      this->layer.getParams().windowShape =
          (WindowShape)(windowShapeCombo.getSelectedId() - 1);
    };

    cutoffSlider.onValueChange = [this] {
      this->layer.getParams().filterCutoff = (float)cutoffSlider.getValue();
    };
    resSlider.onValueChange = [this] {
      this->layer.getParams().filterResonance = (float)resSlider.getValue();
    };

    lowNoteSlider.onValueChange = [this] {
      this->layer.getParams().lowNote = (int)lowNoteSlider.getValue();
    };
    highNoteSlider.onValueChange = [this] {
      this->layer.getParams().highNote = (int)highNoteSlider.getValue();
    };

    atkSlider.onValueChange = [this] {
      this->layer.getParams().attack = (float)atkSlider.getValue();
    };
    decSlider.onValueChange = [this] {
      this->layer.getParams().decay = (float)decSlider.getValue();
    };
    susSlider.onValueChange = [this] {
      this->layer.getParams().sustain = (float)susSlider.getValue();
    };
    relSlider.onValueChange = [this] {
      this->layer.getParams().release = (float)relSlider.getValue();
    };

    startTimerHz(30);
  }

  // File drag & drop support
  bool isInterestedInFileDrag(const juce::StringArray &files) override {
    for (auto &file : files) {
      if (file.endsWithIgnoreCase(".wav") || file.endsWithIgnoreCase(".aif") ||
          file.endsWithIgnoreCase(".aiff") || file.endsWithIgnoreCase(".mp3")) {
        return true;
      }
    }
    return false;
  }

  void fileDragEnter(const juce::StringArray &files, int x, int y) override {
    isDragging = true;
    repaint();
  }

  void fileDragExit(const juce::StringArray &files) override {
    isDragging = false;
    repaint();
  }

  void filesDropped(const juce::StringArray &files, int x, int y) override {
    isDragging = false;
    auto file = juce::File(files[0]);
    if (file.existsAsFile()) {
      layer.loadSample(file);
      sampleNameLabel.setText(file.getFileName().toUpperCase(),
                              juce::dontSendNotification);
      viewer.repaint();
    }
    repaint();
  }

  void timerCallback() override {
    viewer.repaint();

    // Dynamic synchronization from processor to UI sliders when not manually
    // dragging
    if (!posSlider.isMouseButtonDown())
      posSlider.setValue(layer.getParams().position,
                         juce::dontSendNotification);
    if (!sizeSlider.isMouseButtonDown())
      sizeSlider.setValue(layer.getParams().size, juce::dontSendNotification);
    if (!densSlider.isMouseButtonDown())
      densSlider.setValue(layer.getParams().density,
                          juce::dontSendNotification);
    if (!pitchSlider.isMouseButtonDown())
      pitchSlider.setValue(layer.getParams().pitch, juce::dontSendNotification);

    if (!posRandSlider.isMouseButtonDown())
      posRandSlider.setValue(layer.getParams().posRandomness,
                             juce::dontSendNotification);
    if (!pitchRandSlider.isMouseButtonDown())
      pitchRandSlider.setValue(layer.getParams().pitchRandomness,
                               juce::dontSendNotification);
    if (!sizeRandSlider.isMouseButtonDown())
      sizeRandSlider.setValue(layer.getParams().sizeRandomness,
                              juce::dontSendNotification);

    if (!panSlider.isMouseButtonDown())
      panSlider.setValue(layer.getParams().pan, juce::dontSendNotification);
    if (!spreadSlider.isMouseButtonDown())
      spreadSlider.setValue(layer.getParams().spread,
                            juce::dontSendNotification);

    if (!scanSpeedSlider.isMouseButtonDown())
      scanSpeedSlider.setValue(layer.getParams().scanSpeed,
                               juce::dontSendNotification);
    if (!revProbSlider.isMouseButtonDown())
      revProbSlider.setValue(layer.getParams().reverseProbability,
                             juce::dontSendNotification);

    if (!cutoffSlider.isMouseButtonDown())
      cutoffSlider.setValue(layer.getParams().filterCutoff,
                            juce::dontSendNotification);
    if (!resSlider.isMouseButtonDown())
      resSlider.setValue(layer.getParams().filterResonance,
                         juce::dontSendNotification);

    if (!lowNoteSlider.isMouseButtonDown())
      lowNoteSlider.setValue(layer.getParams().lowNote,
                             juce::dontSendNotification);
    if (!highNoteSlider.isMouseButtonDown())
      highNoteSlider.setValue(layer.getParams().highNote,
                              juce::dontSendNotification);

    if (!atkSlider.isMouseButtonDown())
      atkSlider.setValue(layer.getParams().attack, juce::dontSendNotification);
    if (!decSlider.isMouseButtonDown())
      decSlider.setValue(layer.getParams().decay, juce::dontSendNotification);
    if (!susSlider.isMouseButtonDown())
      susSlider.setValue(layer.getParams().sustain, juce::dontSendNotification);
    if (!relSlider.isMouseButtonDown())
      relSlider.setValue(layer.getParams().release, juce::dontSendNotification);

    int expectedShapeId = (int)layer.getParams().windowShape + 1;
    if (windowShapeCombo.getSelectedId() != expectedShapeId)
      windowShapeCombo.setSelectedId(expectedShapeId,
                                     juce::dontSendNotification);

    juce::String currentPath = layer.getCurrentSamplePath();
    if (currentPath.isEmpty()) {
      if (sampleNameLabel.getText() != "READY FOR SAMPLE")
        sampleNameLabel.setText("READY FOR SAMPLE", juce::dontSendNotification);
    } else {
      juce::File f(currentPath);
      juce::String displayName = f.getFileName().toUpperCase();
      if (sampleNameLabel.getText() != displayName)
        sampleNameLabel.setText(displayName, juce::dontSendNotification);
    }
  }

  void paint(juce::Graphics &g) override {
    auto area = getLocalBounds().toFloat().reduced(5);

    juce::ColourGradient bgGrad(
        juce::Colours::white.withAlpha(0.4f), area.getTopLeft(),
        juce::Colours::white.withAlpha(0.15f), area.getBottomRight(), false);
    g.setGradientFill(bgGrad);
    g.fillRoundedRectangle(area, 10.0f);

    if (isDragging) {
      g.setColour(juce::Colour(9, 136, 131)
                      .withAlpha(0.8f)); // Active glass drag outline
      g.drawRoundedRectangle(area, 10.0f, 3.0f);
      g.setColour(juce::Colour(9, 136, 131).withAlpha(0.15f));
      g.fillRoundedRectangle(area, 10.0f);
    } else {
      g.setColour(juce::Colours::white);
      g.drawRoundedRectangle(area, 10.0f, 1.5f);
    }
  }

  void resized() override {
    auto area = getLocalBounds().reduced(20);
    auto top = area.removeFromTop(40);
    loadButton.setBounds(top.removeFromLeft(120).reduced(0, 5));
    top.removeFromLeft(10);
    clearButton.setBounds(top.removeFromLeft(70).reduced(0, 5));
    sampleNameLabel.setBounds(top.reduced(15, 0));

    viewer.setBounds(area.removeFromTop(130).reduced(0, 10));

    area.removeFromTop(10);

    auto granArea = area.removeFromTop(160);
    int colW = granArea.getWidth() / 8;

    auto envGraphicArea = granArea.removeFromRight(colW * 2).reduced(6);
    envVisualizer.setBounds(envGraphicArea);

    auto coreRow = granArea.removeFromTop(granArea.getHeight() / 2);
    posSlider.setBounds(coreRow.removeFromLeft(colW).reduced(6));
    sizeSlider.setBounds(coreRow.removeFromLeft(colW).reduced(6));
    densSlider.setBounds(coreRow.removeFromLeft(colW).reduced(6));
    pitchSlider.setBounds(coreRow.removeFromLeft(colW).reduced(6));
    panSlider.setBounds(coreRow.removeFromLeft(colW).reduced(6));
    spreadSlider.setBounds(coreRow.removeFromLeft(colW).reduced(6));

    auto jitterRow = granArea;
    posRandSlider.setBounds(jitterRow.removeFromLeft(colW).reduced(6));
    pitchRandSlider.setBounds(jitterRow.removeFromLeft(colW).reduced(6));
    sizeRandSlider.setBounds(jitterRow.removeFromLeft(colW).reduced(6));
    scanSpeedSlider.setBounds(jitterRow.removeFromLeft(colW).reduced(6));
    revProbSlider.setBounds(jitterRow.removeFromLeft(colW).reduced(6));
    windowShapeCombo.setBounds(jitterRow.removeFromLeft(colW).reduced(12, 20));

    area.removeFromTop(10);

    auto bottomRow = area.removeFromTop(120);

    auto filter1Area = bottomRow.removeFromLeft(colW);
    cutoffSlider.setBounds(
        filter1Area.withSizeKeepingCentre(colW, 80).reduced(6));

    auto filter2Area = bottomRow.removeFromLeft(colW);
    resSlider.setBounds(
        filter2Area.withSizeKeepingCentre(colW, 80).reduced(6));

    auto lowNoteArea = bottomRow.removeFromLeft(colW);
    lowNoteSlider.setBounds(lowNoteArea.withSizeKeepingCentre(colW, 80).reduced(6));

    auto highNoteArea = bottomRow.removeFromLeft(colW);
    highNoteSlider.setBounds(highNoteArea.withSizeKeepingCentre(colW, 80).reduced(6));

    atkSlider.setBounds(bottomRow.removeFromLeft(colW).reduced(6));
    decSlider.setBounds(bottomRow.removeFromLeft(colW).reduced(6));
    susSlider.setBounds(bottomRow.removeFromLeft(colW).reduced(6));
    relSlider.setBounds(bottomRow.removeFromLeft(colW).reduced(6));

    for (auto &pair : sliderLabels) {
      auto sb = pair.first->getBounds();
      pair.first->setBounds(sb.withTrimmedBottom(15));
      pair.second->setBounds(sb.withTop(sb.getBottom() - 15).withHeight(15));
    }
  }

private:
  void setupSectionLabel(juce::Label &l, const juce::String &text) {
    addAndMakeVisible(l);
    l.setText(text, juce::dontSendNotification);
    l.setFont(juce::FontOptions(15.0f).withStyle("Bold"));
    l.setColour(juce::Label::textColourId, juce::Colour(0xff2d3436));
    l.setJustificationType(juce::Justification::left);
  }

  void setupSlider(juce::Slider &s, const juce::String &name, float min,
                   float max, float val, float defaultVal,
                   bool rotary = false) {
    addAndMakeVisible(s);
    s.setRange(min, max);
    s.setValue(val);
    s.setSliderStyle(rotary ? juce::Slider::RotaryHorizontalVerticalDrag
                            : juce::Slider::LinearVertical);
    s.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    s.setDoubleClickReturnValue(true, defaultVal);

    auto *label = labels.add(new juce::Label(name, name));
    addAndMakeVisible(label);
    label->setFont(juce::FontOptions(11.0f).withStyle("Bold"));
    label->setColour(juce::Label::textColourId, juce::Colours::white);
    label->setJustificationType(juce::Justification::centred);
    sliderLabels[&s] = label;
  }

  GranularSynthAudioProcessor &audioProcessor;
  int layerIndex;
  GranularLayer &layer;
  WaveformViewer viewer;
  EnvelopeVisualizer envVisualizer;
  juce::TextButton loadButton;
  juce::TextButton clearButton;
  juce::Label sampleNameLabel;
  juce::Label granLabel, filterLabel, envLabel;
  MappedSlider posSlider, sizeSlider, densSlider, pitchSlider;
  MappedSlider posRandSlider, pitchRandSlider, sizeRandSlider;
  MappedSlider panSlider, spreadSlider;
  MappedSlider scanSpeedSlider, revProbSlider;
  juce::ComboBox windowShapeCombo;
  MappedSlider atkSlider, decSlider, susSlider, relSlider;
  MappedSlider cutoffSlider, resSlider;
  MappedSlider lowNoteSlider, highNoteSlider;
  juce::OwnedArray<juce::Label> labels;
  std::map<juce::Slider *, juce::Label *> sliderLabels;
  std::unique_ptr<juce::FileChooser> chooser;
  bool isDragging = false;
};

class EffectsUI : public juce::Component, public juce::Timer {
public:
  EffectsUI(GranularSynthAudioProcessor &p) : audioProcessor(p) {
    reverbSizeSlider.init("global/reverbSize", audioProcessor);
    reverbWetSlider.init("global/reverbWet", audioProcessor);
    chorusRateSlider.init("global/chorusRate", audioProcessor);
    chorusDepthSlider.init("global/chorusDepth", audioProcessor);
    chorusMixSlider.init("global/chorusMix", audioProcessor);
    filterCutoffSlider.init("global/filterCutoff", audioProcessor);
    filterResSlider.init("global/filterResonance", audioProcessor);
    limiterThresholdSlider.init("global/limiterThreshold", audioProcessor);
    limiterReleaseSlider.init("global/limiterRelease", audioProcessor);

    delayTimeSlider.init("global/delayTime", audioProcessor);
    delayFeedbackSlider.init("global/delayFeedback", audioProcessor);
    delayMixSlider.init("global/delayMix", audioProcessor);

    phaserRateSlider.init("global/phaserRate", audioProcessor);
    phaserDepthSlider.init("global/phaserDepth", audioProcessor);
    phaserCenterFreqSlider.init("global/phaserCenterFreq", audioProcessor);
    phaserMixSlider.init("global/phaserMix", audioProcessor);

    // 1. Reverb Bypass & Sliders
    addAndMakeVisible(reverbBypassBtn);
    reverbBypassBtn.setClickingTogglesState(true);
    reverbBypassBtn.setToggleState(!audioProcessor.getFXParams().reverbBypass,
                                   juce::dontSendNotification);
    reverbBypassBtn.setColour(juce::TextButton::buttonColourId,
                              juce::Colour(45, 52, 54).withAlpha(0.4f));
    reverbBypassBtn.setColour(
        juce::TextButton::buttonOnColourId,
        juce::Colour(9, 136, 131).withAlpha(0.85f)); // Soft glowing teal
    reverbBypassBtn.setColour(juce::TextButton::textColourOffId,
                              juce::Colour(120, 120, 120));
    reverbBypassBtn.setColour(juce::TextButton::textColourOnId,
                              juce::Colours::white);
    reverbBypassBtn.onClick = [this] {
      audioProcessor.getFXParams().reverbBypass =
          !reverbBypassBtn.getToggleState();
    };

    setupSlider(reverbSizeSlider, "REV SIZE",
                audioProcessor.getReverbParams().roomSize, 0.5f);
    reverbSizeSlider.onValueChange = [this] {
      audioProcessor.getReverbParams().roomSize =
          (float)reverbSizeSlider.getValue();
    };

    setupSlider(reverbWetSlider, "REV WET",
                audioProcessor.getReverbParams().wetLevel, 0.33f);
    reverbWetSlider.onValueChange = [this] {
      audioProcessor.getReverbParams().wetLevel =
          (float)reverbWetSlider.getValue();
    };

    // 2. Chorus Bypass & Sliders
    addAndMakeVisible(chorusBypassBtn);
    chorusBypassBtn.setClickingTogglesState(true);
    chorusBypassBtn.setToggleState(!audioProcessor.getFXParams().chorusBypass,
                                   juce::dontSendNotification);
    chorusBypassBtn.setColour(juce::TextButton::buttonColourId,
                              juce::Colour(45, 52, 54).withAlpha(0.4f));
    chorusBypassBtn.setColour(juce::TextButton::buttonOnColourId,
                              juce::Colour(9, 136, 131).withAlpha(0.85f));
    chorusBypassBtn.setColour(juce::TextButton::textColourOffId,
                              juce::Colour(120, 120, 120));
    chorusBypassBtn.setColour(juce::TextButton::textColourOnId,
                              juce::Colours::white);
    chorusBypassBtn.onClick = [this] {
      audioProcessor.getFXParams().chorusBypass =
          !chorusBypassBtn.getToggleState();
    };

    setupSlider(chorusRateSlider, "CHO RATE",
                audioProcessor.getFXParams().chorusRate, 1.0f, 0.1f, 10.0f);
    chorusRateSlider.onValueChange = [this] {
      audioProcessor.getFXParams().chorusRate =
          (float)chorusRateSlider.getValue();
    };

    setupSlider(chorusDepthSlider, "CHO DEPTH",
                audioProcessor.getFXParams().chorusDepth, 0.2f, 0.0f, 1.0f);
    chorusDepthSlider.onValueChange = [this] {
      audioProcessor.getFXParams().chorusDepth =
          (float)chorusDepthSlider.getValue();
    };

    setupSlider(chorusMixSlider, "CHO MIX",
                audioProcessor.getFXParams().chorusMix, 0.0f, 0.0f, 1.0f);
    chorusMixSlider.onValueChange = [this] {
      audioProcessor.getFXParams().chorusMix =
          (float)chorusMixSlider.getValue();
    };

    // 2.5 Delay Bypass & Sliders
    addAndMakeVisible(delayBypassBtn);
    delayBypassBtn.setClickingTogglesState(true);
    delayBypassBtn.setToggleState(!audioProcessor.getFXParams().delayBypass,
                                  juce::dontSendNotification);
    delayBypassBtn.setColour(juce::TextButton::buttonColourId,
                             juce::Colour(45, 52, 54).withAlpha(0.4f));
    delayBypassBtn.setColour(juce::TextButton::buttonOnColourId,
                             juce::Colour(9, 136, 131).withAlpha(0.85f));
    delayBypassBtn.setColour(juce::TextButton::textColourOffId,
                             juce::Colour(120, 120, 120));
    delayBypassBtn.setColour(juce::TextButton::textColourOnId,
                             juce::Colours::white);
    delayBypassBtn.onClick = [this] {
      audioProcessor.getFXParams().delayBypass =
          !delayBypassBtn.getToggleState();
    };

    setupSlider(delayTimeSlider, "DLY TIME",
                audioProcessor.getFXParams().delayTime, 250.0f, 10.0f, 2000.0f);
    delayTimeSlider.onValueChange = [this] {
      audioProcessor.getFXParams().delayTime =
          (float)delayTimeSlider.getValue();
    };

    setupSlider(delayFeedbackSlider, "DLY FDBK",
                audioProcessor.getFXParams().delayFeedback, 0.3f, 0.0f, 0.95f);
    delayFeedbackSlider.onValueChange = [this] {
      audioProcessor.getFXParams().delayFeedback =
          (float)delayFeedbackSlider.getValue();
    };

    setupSlider(delayMixSlider, "DLY MIX",
                audioProcessor.getFXParams().delayMix, 0.0f, 0.0f, 1.0f);
    delayMixSlider.onValueChange = [this] {
      audioProcessor.getFXParams().delayMix = (float)delayMixSlider.getValue();
    };

    // 2.6 Phaser Bypass & Sliders
    addAndMakeVisible(phaserBypassBtn);
    phaserBypassBtn.setClickingTogglesState(true);
    phaserBypassBtn.setToggleState(!audioProcessor.getFXParams().phaserBypass,
                                   juce::dontSendNotification);
    phaserBypassBtn.setColour(juce::TextButton::buttonColourId,
                              juce::Colour(45, 52, 54).withAlpha(0.4f));
    phaserBypassBtn.setColour(juce::TextButton::buttonOnColourId,
                              juce::Colour(9, 136, 131).withAlpha(0.85f));
    phaserBypassBtn.setColour(juce::TextButton::textColourOffId,
                              juce::Colour(120, 120, 120));
    phaserBypassBtn.setColour(juce::TextButton::textColourOnId,
                              juce::Colours::white);
    phaserBypassBtn.onClick = [this] {
      audioProcessor.getFXParams().phaserBypass =
          !phaserBypassBtn.getToggleState();
    };

    setupSlider(phaserRateSlider, "PHS RATE",
                audioProcessor.getFXParams().phaserRate, 0.5f, 0.01f, 10.0f);
    phaserRateSlider.onValueChange = [this] {
      audioProcessor.getFXParams().phaserRate =
          (float)phaserRateSlider.getValue();
    };

    setupSlider(phaserDepthSlider, "PHS DPTH",
                audioProcessor.getFXParams().phaserDepth, 0.5f, 0.0f, 1.0f);
    phaserDepthSlider.onValueChange = [this] {
      audioProcessor.getFXParams().phaserDepth =
          (float)phaserDepthSlider.getValue();
    };

    setupSlider(phaserCenterFreqSlider, "PHS FREQ",
                audioProcessor.getFXParams().phaserCenterFreq, 1000.0f, 50.0f,
                5000.0f);
    phaserCenterFreqSlider.onValueChange = [this] {
      audioProcessor.getFXParams().phaserCenterFreq =
          (float)phaserCenterFreqSlider.getValue();
    };

    setupSlider(phaserMixSlider, "PHS MIX",
                audioProcessor.getFXParams().phaserMix, 0.0f, 0.0f, 1.0f);
    phaserMixSlider.onValueChange = [this] {
      audioProcessor.getFXParams().phaserMix =
          (float)phaserMixSlider.getValue();
    };

    // 3. Filter Bypass & Sliders
    addAndMakeVisible(filterBypassBtn);
    filterBypassBtn.setClickingTogglesState(true);
    filterBypassBtn.setToggleState(!audioProcessor.getFXParams().filterBypass,
                                   juce::dontSendNotification);
    filterBypassBtn.setColour(juce::TextButton::buttonColourId,
                              juce::Colour(45, 52, 54).withAlpha(0.4f));
    filterBypassBtn.setColour(juce::TextButton::buttonOnColourId,
                              juce::Colour(9, 136, 131).withAlpha(0.85f));
    filterBypassBtn.setColour(juce::TextButton::textColourOffId,
                              juce::Colour(120, 120, 120));
    filterBypassBtn.setColour(juce::TextButton::textColourOnId,
                              juce::Colours::white);
    filterBypassBtn.onClick = [this] {
      audioProcessor.getFXParams().filterBypass =
          !filterBypassBtn.getToggleState();
    };

    setupSlider(filterCutoffSlider, "MASTER HP/LP",
                audioProcessor.getFXParams().filterCutoff, 20000.0f, 20.0f,
                20000.0f);
    filterCutoffSlider.setSkewFactorFromMidPoint(1000.0f);
    filterCutoffSlider.onValueChange = [this] {
      audioProcessor.getFXParams().filterCutoff =
          (float)filterCutoffSlider.getValue();
    };

    setupSlider(filterResSlider, "MST RES",
                audioProcessor.getFXParams().filterResonance, 0.1f, 0.1f, 2.0f);
    filterResSlider.onValueChange = [this] {
      audioProcessor.getFXParams().filterResonance =
          (float)filterResSlider.getValue();
    };

    // 4. Limiter Bypass & Sliders
    addAndMakeVisible(limiterBypassBtn);
    limiterBypassBtn.setClickingTogglesState(true);
    limiterBypassBtn.setToggleState(!audioProcessor.getFXParams().limiterBypass,
                                    juce::dontSendNotification);
    limiterBypassBtn.setColour(juce::TextButton::buttonColourId,
                               juce::Colour(45, 52, 54).withAlpha(0.4f));
    limiterBypassBtn.setColour(juce::TextButton::buttonOnColourId,
                               juce::Colour(9, 136, 131).withAlpha(0.85f));
    limiterBypassBtn.setColour(juce::TextButton::textColourOffId,
                               juce::Colour(120, 120, 120));
    limiterBypassBtn.setColour(juce::TextButton::textColourOnId,
                               juce::Colours::white);
    limiterBypassBtn.onClick = [this] {
      audioProcessor.getFXParams().limiterBypass =
          !limiterBypassBtn.getToggleState();
    };

    setupSlider(limiterThresholdSlider, "LMT THRESH",
                audioProcessor.getFXParams().limiterThreshold, 0.0f, -40.0f,
                0.0f);
    limiterThresholdSlider.onValueChange = [this] {
      audioProcessor.getFXParams().limiterThreshold =
          (float)limiterThresholdSlider.getValue();
    };

    setupSlider(limiterReleaseSlider, "LMT REL",
                audioProcessor.getFXParams().limiterRelease, 100.0f, 1.0f,
                500.0f);
    limiterReleaseSlider.onValueChange = [this] {
      audioProcessor.getFXParams().limiterRelease =
          (float)limiterReleaseSlider.getValue();
    };

    startTimerHz(30);
  }

  void timerCallback() override {
    if (!reverbSizeSlider.isMouseButtonDown())
      reverbSizeSlider.setValue(audioProcessor.getReverbParams().roomSize,
                                juce::dontSendNotification);
    if (!reverbWetSlider.isMouseButtonDown())
      reverbWetSlider.setValue(audioProcessor.getReverbParams().wetLevel,
                               juce::dontSendNotification);

    if (!chorusRateSlider.isMouseButtonDown())
      chorusRateSlider.setValue(audioProcessor.getFXParams().chorusRate,
                                juce::dontSendNotification);
    if (!chorusDepthSlider.isMouseButtonDown())
      chorusDepthSlider.setValue(audioProcessor.getFXParams().chorusDepth,
                                 juce::dontSendNotification);
    if (!chorusMixSlider.isMouseButtonDown())
      chorusMixSlider.setValue(audioProcessor.getFXParams().chorusMix,
                               juce::dontSendNotification);

    if (!filterCutoffSlider.isMouseButtonDown())
      filterCutoffSlider.setValue(audioProcessor.getFXParams().filterCutoff,
                                  juce::dontSendNotification);
    if (!filterResSlider.isMouseButtonDown())
      filterResSlider.setValue(audioProcessor.getFXParams().filterResonance,
                               juce::dontSendNotification);

    if (!limiterThresholdSlider.isMouseButtonDown())
      limiterThresholdSlider.setValue(
          audioProcessor.getFXParams().limiterThreshold,
          juce::dontSendNotification);
    if (!limiterReleaseSlider.isMouseButtonDown())
      limiterReleaseSlider.setValue(audioProcessor.getFXParams().limiterRelease,
                                    juce::dontSendNotification);

    if (!delayTimeSlider.isMouseButtonDown())
      delayTimeSlider.setValue(audioProcessor.getFXParams().delayTime,
                               juce::dontSendNotification);
    if (!delayFeedbackSlider.isMouseButtonDown())
      delayFeedbackSlider.setValue(audioProcessor.getFXParams().delayFeedback,
                                   juce::dontSendNotification);
    if (!delayMixSlider.isMouseButtonDown())
      delayMixSlider.setValue(audioProcessor.getFXParams().delayMix,
                              juce::dontSendNotification);

    if (!phaserRateSlider.isMouseButtonDown())
      phaserRateSlider.setValue(audioProcessor.getFXParams().phaserRate,
                                juce::dontSendNotification);
    if (!phaserDepthSlider.isMouseButtonDown())
      phaserDepthSlider.setValue(audioProcessor.getFXParams().phaserDepth,
                                 juce::dontSendNotification);
    if (!phaserCenterFreqSlider.isMouseButtonDown())
      phaserCenterFreqSlider.setValue(
          audioProcessor.getFXParams().phaserCenterFreq,
          juce::dontSendNotification);
    if (!phaserMixSlider.isMouseButtonDown())
      phaserMixSlider.setValue(audioProcessor.getFXParams().phaserMix,
                               juce::dontSendNotification);

    // Sync bypass button states
    bool expectedReverbToggle = !audioProcessor.getFXParams().reverbBypass;
    if (reverbBypassBtn.getToggleState() != expectedReverbToggle)
      reverbBypassBtn.setToggleState(expectedReverbToggle,
                                     juce::dontSendNotification);

    bool expectedChorusToggle = !audioProcessor.getFXParams().chorusBypass;
    if (chorusBypassBtn.getToggleState() != expectedChorusToggle)
      chorusBypassBtn.setToggleState(expectedChorusToggle,
                                     juce::dontSendNotification);

    bool expectedFilterToggle = !audioProcessor.getFXParams().filterBypass;
    if (filterBypassBtn.getToggleState() != expectedFilterToggle)
      filterBypassBtn.setToggleState(expectedFilterToggle,
                                     juce::dontSendNotification);

    bool expectedLimiterToggle = !audioProcessor.getFXParams().limiterBypass;
    if (limiterBypassBtn.getToggleState() != expectedLimiterToggle)
      limiterBypassBtn.setToggleState(expectedLimiterToggle,
                                      juce::dontSendNotification);

    bool expectedDelayToggle = !audioProcessor.getFXParams().delayBypass;
    if (delayBypassBtn.getToggleState() != expectedDelayToggle)
      delayBypassBtn.setToggleState(expectedDelayToggle,
                                    juce::dontSendNotification);

    bool expectedPhaserToggle = !audioProcessor.getFXParams().phaserBypass;
    if (phaserBypassBtn.getToggleState() != expectedPhaserToggle)
      phaserBypassBtn.setToggleState(expectedPhaserToggle,
                                     juce::dontSendNotification);
  }

  void paint(juce::Graphics &g) override {
    auto area = getLocalBounds().toFloat().reduced(10);

    juce::ColourGradient bgGrad(
        juce::Colours::white.withAlpha(0.4f), area.getTopLeft(),
        juce::Colours::white.withAlpha(0.15f), area.getBottomRight(), false);
    g.setGradientFill(bgGrad);
    g.fillRoundedRectangle(area, 10.0f);

    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(area, 10.0f, 1.5f);

    // Draw horizontal thin line between fx rows
    auto innerArea = getLocalBounds().reduced(40, 20).toFloat();
    float rowHeight = innerArea.getHeight() / 6.0f;
    g.setColour(juce::Colours::white.withAlpha(0.2f));
    for (int i = 1; i < 6; ++i) {
      float y = innerArea.getY() + i * rowHeight;
      g.drawLine(innerArea.getX() - 10.0f, y, innerArea.getRight() + 10.0f, y,
                 1.0f);
    }
  }

  void resized() override {
    auto area = getLocalBounds().reduced(40, 20);

    auto rowHeight = area.getHeight() / 6;

    // Use a uniform slider width for all effects to create a clean
    // left-to-right grid. 4 is the maximum number of sliders in any row
    // (Phaser).
    int sliderW = (area.getWidth() - 70) / 4;

    auto reverbArea = area.removeFromTop(rowHeight);
    auto reverbBtnArea = reverbArea.removeFromLeft(70).reduced(0, 10);
    reverbBypassBtn.setBounds(reverbBtnArea);
    reverbSizeSlider.setBounds(
        reverbArea.removeFromLeft(sliderW).reduced(20, 5));
    reverbWetSlider.setBounds(
        reverbArea.removeFromLeft(sliderW).reduced(20, 5));

    auto delayArea = area.removeFromTop(rowHeight);
    auto delayBtnArea = delayArea.removeFromLeft(70).reduced(0, 10);
    delayBypassBtn.setBounds(delayBtnArea);
    delayTimeSlider.setBounds(delayArea.removeFromLeft(sliderW).reduced(20, 5));
    delayFeedbackSlider.setBounds(
        delayArea.removeFromLeft(sliderW).reduced(20, 5));
    delayMixSlider.setBounds(delayArea.removeFromLeft(sliderW).reduced(20, 5));

    auto chorusArea = area.removeFromTop(rowHeight);
    auto chorusBtnArea = chorusArea.removeFromLeft(70).reduced(0, 10);
    chorusBypassBtn.setBounds(chorusBtnArea);
    chorusRateSlider.setBounds(
        chorusArea.removeFromLeft(sliderW).reduced(20, 5));
    chorusDepthSlider.setBounds(
        chorusArea.removeFromLeft(sliderW).reduced(20, 5));
    chorusMixSlider.setBounds(
        chorusArea.removeFromLeft(sliderW).reduced(20, 5));

    auto phaserArea = area.removeFromTop(rowHeight);
    auto phaserBtnArea = phaserArea.removeFromLeft(70).reduced(0, 10);
    phaserBypassBtn.setBounds(phaserBtnArea);
    phaserRateSlider.setBounds(
        phaserArea.removeFromLeft(sliderW).reduced(20, 5));
    phaserDepthSlider.setBounds(
        phaserArea.removeFromLeft(sliderW).reduced(20, 5));
    phaserCenterFreqSlider.setBounds(
        phaserArea.removeFromLeft(sliderW).reduced(20, 5));
    phaserMixSlider.setBounds(
        phaserArea.removeFromLeft(sliderW).reduced(20, 5));

    auto filterArea = area.removeFromTop(rowHeight);
    auto filterBtnArea = filterArea.removeFromLeft(70).reduced(0, 10);
    filterBypassBtn.setBounds(filterBtnArea);
    filterCutoffSlider.setBounds(
        filterArea.removeFromLeft(sliderW).reduced(20, 5));
    filterResSlider.setBounds(
        filterArea.removeFromLeft(sliderW).reduced(20, 5));

    auto limiterArea = area;
    auto limiterBtnArea = limiterArea.removeFromLeft(70).reduced(0, 10);
    limiterBypassBtn.setBounds(limiterBtnArea);
    limiterThresholdSlider.setBounds(
        limiterArea.removeFromLeft(sliderW).reduced(20, 5));
    limiterReleaseSlider.setBounds(
        limiterArea.removeFromLeft(sliderW).reduced(20, 5));

    for (auto &pair : sliderLabels) {
      auto sb = pair.first->getBounds();
      pair.first->setBounds(sb.withTrimmedBottom(15));
      pair.second->setBounds(sb.withTop(sb.getBottom() - 15).withHeight(15));
    }
  }

private:
  void setupSlider(juce::Slider &s, const juce::String &name, float val,
                   float defaultVal, float min = 0.0f, float max = 1.0f) {
    addAndMakeVisible(s);
    s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    s.setRange(min, max);
    s.setValue(val);
    s.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    s.setDoubleClickReturnValue(true, defaultVal);

    auto *label = labels.add(new juce::Label(name, name));
    addAndMakeVisible(label);
    label->setFont(juce::FontOptions(11.0f).withStyle("Bold"));
    label->setColour(juce::Label::textColourId, juce::Colours::white);
    label->setJustificationType(juce::Justification::centred);
    sliderLabels[&s] = label;
  }

  GranularSynthAudioProcessor &audioProcessor;
  juce::TextButton reverbBypassBtn{"ON"}, chorusBypassBtn{"ON"},
      filterBypassBtn{"ON"}, limiterBypassBtn{"ON"}, delayBypassBtn{"ON"},
      phaserBypassBtn{"ON"};
  MappedSlider reverbSizeSlider, reverbWetSlider;
  MappedSlider chorusRateSlider, chorusDepthSlider, chorusMixSlider;
  MappedSlider delayTimeSlider, delayFeedbackSlider, delayMixSlider;
  MappedSlider phaserRateSlider, phaserDepthSlider, phaserCenterFreqSlider,
      phaserMixSlider;
  MappedSlider filterCutoffSlider, filterResSlider;
  MappedSlider limiterThresholdSlider, limiterReleaseSlider;
  juce::OwnedArray<juce::Label> labels;
  std::map<juce::Slider *, juce::Label *> sliderLabels;
};

class MixerUI : public juce::Component, public juce::Timer {
public:
  MixerUI(GranularSynthAudioProcessor &p) : audioProcessor(p) {
    for (int i = 0; i < 4; ++i) {
      // 1. Volume Sliders
      auto *s = sliders.add(new MappedSlider());
      s->init("layer/" + juce::String(i) + "/volume", audioProcessor);
      addAndMakeVisible(s);
      s->setSliderStyle(juce::Slider::LinearVertical);
      s->setRange(0.0f, 1.0f);
      s->setValue(audioProcessor.getLayer(i).getParams().volume);
      s->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
      s->setDoubleClickReturnValue(true, 0.8f);
      s->onValueChange = [this, i, s] {
        audioProcessor.getLayer(i).getParams().volume = (float)s->getValue();
      };

      // 2. Pan Sliders
      auto *ps = panSliders.add(new MappedSlider());
      ps->init("layer/" + juce::String(i) + "/pan", audioProcessor);
      addAndMakeVisible(ps);
      ps->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
      ps->setRange(-1.0f, 1.0f);
      ps->setValue(audioProcessor.getLayer(i).getParams().pan);
      ps->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
      ps->setDoubleClickReturnValue(true, 0.0f);
      ps->onValueChange = [this, i, ps] {
        audioProcessor.getLayer(i).getParams().pan = (float)ps->getValue();
      };

      // 3. Layer Name Labels
      auto *l = labels.add(new juce::Label("L" + juce::String(i + 1),
                                           "LAYER " + juce::String(i + 1)));
      addAndMakeVisible(l);
      l->setFont(juce::FontOptions(14.0f).withStyle("Bold"));
      l->setColour(juce::Label::textColourId, juce::Colour(0xff2d3436));
      l->setJustificationType(juce::Justification::centred);

      // 4. Pan Labels
      auto *pl =
          panLabels.add(new juce::Label("PL" + juce::String(i + 1), "PAN"));
      addAndMakeVisible(pl);
      pl->setFont(juce::FontOptions(11.0f).withStyle("Bold"));
      pl->setColour(juce::Label::textColourId, juce::Colours::white);
      pl->setJustificationType(juce::Justification::centred);

      // 5. Mute Buttons
      auto *m = muteButtons.add(new juce::TextButton("M"));
      addAndMakeVisible(m);
      m->setClickingTogglesState(true);
      m->setToggleState(audioProcessor.getLayer(i).getParams().isMuted,
                        juce::dontSendNotification);
      m->setColour(juce::TextButton::buttonColourId,
                   juce::Colours::transparentBlack);
      m->setColour(juce::TextButton::buttonOnColourId,
                   juce::Colour(220, 80, 80).withAlpha(0.8f)); // Clean soft red
      m->setColour(juce::TextButton::textColourOffId, juce::Colour(0xff2d3436));
      m->setColour(juce::TextButton::textColourOnId, juce::Colours::white);
      m->onClick = [this, i, m] {
        audioProcessor.getLayer(i).getParams().isMuted = m->getToggleState();
      };

      // 6. Solo Buttons
      auto *sol = soloButtons.add(new juce::TextButton("S"));
      addAndMakeVisible(sol);
      sol->setClickingTogglesState(true);
      sol->setToggleState(audioProcessor.getLayer(i).getParams().isSoloed,
                          juce::dontSendNotification);
      sol->setColour(juce::TextButton::buttonColourId,
                     juce::Colours::transparentBlack);
      sol->setColour(
          juce::TextButton::buttonOnColourId,
          juce::Colour(230, 160, 40).withAlpha(0.8f)); // Clean soft amber
      sol->setColour(juce::TextButton::textColourOffId,
                     juce::Colour(0xff2d3436));
      sol->setColour(juce::TextButton::textColourOnId, juce::Colours::white);
      sol->onClick = [this, i, sol] {
        audioProcessor.getLayer(i).getParams().isSoloed = sol->getToggleState();
      };
    }

    startTimerHz(30);
  }

  void timerCallback() override {
    for (int i = 0; i < 4; ++i) {
      if (i < sliders.size() && sliders[i] != nullptr &&
          !sliders[i]->isMouseButtonDown()) {
        sliders[i]->setValue(audioProcessor.getLayer(i).getParams().volume,
                             juce::dontSendNotification);
      }
      if (i < panSliders.size() && panSliders[i] != nullptr &&
          !panSliders[i]->isMouseButtonDown()) {
        panSliders[i]->setValue(audioProcessor.getLayer(i).getParams().pan,
                                juce::dontSendNotification);
      }
      if (i < muteButtons.size() && muteButtons[i] != nullptr) {
        muteButtons[i]->setToggleState(
            audioProcessor.getLayer(i).getParams().isMuted,
            juce::dontSendNotification);
      }
      if (i < soloButtons.size() && soloButtons[i] != nullptr) {
        soloButtons[i]->setToggleState(
            audioProcessor.getLayer(i).getParams().isSoloed,
            juce::dontSendNotification);
      }
    }
    repaint();
  }

  void paint(juce::Graphics &g) override {
    auto area = getLocalBounds().toFloat().reduced(10);

    juce::ColourGradient bgGrad(
        juce::Colours::white.withAlpha(0.4f), area.getTopLeft(),
        juce::Colours::white.withAlpha(0.15f), area.getBottomRight(), false);
    g.setGradientFill(bgGrad);
    g.fillRoundedRectangle(area, 10.0f);

    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(area, 10.0f, 1.5f);

    // Draw active level meters next to each layer channel strip
    auto mixerArea = getLocalBounds().reduced(50);
    int layerW = mixerArea.getWidth() / 4;

    for (int i = 0; i < 4; ++i) {
      auto layerArea = mixerArea.removeFromLeft(layerW);

      // Level Meter bounds (moved down to align precisely with volume faders)
      int meterW = 6;
      int meterX = layerArea.getRight() - 25;

      // Calculate vertical volume fader area manually to align meter perfectly
      auto faderArea = layerArea;
      faderArea.removeFromBottom(35); // Remove mute/solo buttons and gap
      faderArea.removeFromTop(
          80); // Remove title label, pan knob, and pan label

      int meterY = faderArea.getY() + 10;
      int meterH = faderArea.getHeight() - 20;

      // Draw meter track background
      g.setColour(juce::Colours::black.withAlpha(0.12f));
      g.fillRoundedRectangle((float)meterX, (float)meterY, (float)meterW,
                             (float)meterH, 3.0f);

      // Track outline
      g.setColour(juce::Colours::white.withAlpha(0.2f));
      g.drawRoundedRectangle((float)meterX, (float)meterY, (float)meterW,
                             (float)meterH, 3.0f, 1.0f);

      // Smooth decay tracking
      float rawLevel = audioProcessor.getLayer(i).getCurrentLevel();
      float currentVisual = visualLevels[i];
      if (rawLevel > currentVisual) {
        currentVisual = rawLevel;
      } else {
        currentVisual -= 0.08f; // Smooth linear decay
        if (currentVisual < 0.0f)
          currentVisual = 0.0f;
      }
      visualLevels[i] = currentVisual;

      float levelHeight = currentVisual * meterH;
      if (levelHeight > meterH)
        levelHeight = (float)meterH;

      if (levelHeight > 0.0f) {
        // Glowing teal HSL gradient
        juce::Colour accentColor = juce::Colour(9, 136, 131);
        juce::ColourGradient meterGrad(
            accentColor.withAlpha(0.85f), (float)meterX,
            (float)(meterY + meterH),
            accentColor.brighter(0.4f).withAlpha(0.95f), (float)meterX,
            (float)(meterY + meterH - levelHeight), false);
        g.setGradientFill(meterGrad);
        g.fillRoundedRectangle(
            area.getIntersection(juce::Rectangle<float>(
                (float)meterX, (float)(meterY + meterH - levelHeight),
                (float)meterW, levelHeight)),
            3.0f);

        // Glow aura
        g.setColour(accentColor.withAlpha(0.2f * currentVisual));
        g.drawRoundedRectangle(
            (float)meterX - 1.5f, (float)(meterY + meterH - levelHeight) - 1.5f,
            (float)meterW + 3.0f, levelHeight + 3.0f, 3.0f, 1.5f);
      }
    }
  }

  void resized() override {
    auto area = getLocalBounds().reduced(50);
    int sliderW = area.getWidth() / 4;

    for (int i = 0; i < 4; ++i) {
      auto layerArea = area.removeFromLeft(sliderW);

      // 1. Mute/Solo buttons at the bottom (height = 25)
      auto buttonArea = layerArea.removeFromBottom(25).reduced(15, 0);
      auto leftBtn =
          buttonArea.removeFromLeft(buttonArea.getWidth() / 2).reduced(2, 0);
      auto rightBtn = buttonArea.reduced(2, 0);

      muteButtons[i]->setBounds(leftBtn);
      soloButtons[i]->setBounds(rightBtn);

      layerArea.removeFromBottom(10);

      // 2. Layer Title Label at the top (height = 20)
      auto titleArea = layerArea.removeFromTop(20);
      labels[i]->setBounds(titleArea);

      // 3. Pan knob (height = 45) and its Label "PAN" (height = 15)
      auto panKnobArea = layerArea.removeFromTop(45).reduced(25, 0);
      panSliders[i]->setBounds(panKnobArea);

      auto panLabelArea = layerArea.removeFromTop(15);
      panLabels[i]->setBounds(panLabelArea);

      // 4. Volume fader in the remaining space
      sliders[i]->setBounds(layerArea.reduced(20, 10));
    }
  }

private:
  GranularSynthAudioProcessor &audioProcessor;
  juce::OwnedArray<MappedSlider> sliders;
  juce::OwnedArray<MappedSlider> panSliders;
  juce::OwnedArray<juce::Label> labels;
  juce::OwnedArray<juce::Label> panLabels;
  juce::OwnedArray<juce::TextButton> muteButtons;
  juce::OwnedArray<juce::TextButton> soloButtons;
  float visualLevels[4] = {0.0f, 0.0f, 0.0f, 0.0f};
};

class XYPadComponent : public juce::Component {
public:
  XYPadComponent(GranularSynthAudioProcessor& p) : audioProcessor(p) {}

  void paint(juce::Graphics& g) override {
    auto bounds = getLocalBounds().toFloat();
    
    // Background
    g.setColour(juce::Colours::black.withAlpha(0.2f));
    g.fillRoundedRectangle(bounds, 10.0f);
    
    g.setColour(juce::Colours::white.withAlpha(0.2f));
    g.drawRoundedRectangle(bounds, 10.0f, 2.0f);
    
    // Crosshairs
    g.setColour(juce::Colours::white.withAlpha(0.1f));
    g.drawLine(bounds.getCentreX(), bounds.getY(), bounds.getCentreX(), bounds.getBottom(), 2.0f);
    g.drawLine(bounds.getX(), bounds.getCentreY(), bounds.getRight(), bounds.getCentreY(), 2.0f);
    
    // Labels
    g.setColour(juce::Colours::white.withAlpha(0.5f));
    g.setFont(juce::FontOptions(18.0f).withStyle("Bold"));
    g.drawText("1", juce::Rectangle<float>(bounds.getX() + 15, bounds.getY() + 15, 30, 30), juce::Justification::centred);
    g.drawText("2", juce::Rectangle<float>(bounds.getRight() - 45, bounds.getY() + 15, 30, 30), juce::Justification::centred);
    g.drawText("3", juce::Rectangle<float>(bounds.getX() + 15, bounds.getBottom() - 45, 30, 30), juce::Justification::centred);
    g.drawText("4", juce::Rectangle<float>(bounds.getRight() - 45, bounds.getBottom() - 45, 30, 30), juce::Justification::centred);

    // Draw the puck
    float x = 0.5f;
    float y = 0.5f;
    
    float v1 = audioProcessor.getLayer(0).getParams().volume;
    float v2 = audioProcessor.getLayer(1).getParams().volume;
    float v3 = audioProcessor.getLayer(2).getParams().volume;
    float v4 = audioProcessor.getLayer(3).getParams().volume;
    
    float total = v1 + v2 + v3 + v4;
    if (total > 0.0f) {
      x = (v2 + v4) / total;
      y = (v3 + v4) / total;
    }
    
    float puckX = bounds.getX() + x * bounds.getWidth();
    float puckY = bounds.getY() + y * bounds.getHeight();
    
    juce::Rectangle<float> puck(puckX - 15.0f, puckY - 15.0f, 30.0f, 30.0f);
    
    g.setColour(juce::Colour(9, 136, 131).withAlpha(0.85f));
    g.fillEllipse(puck);
    
    g.setColour(juce::Colours::white);
    g.drawEllipse(puck, 2.0f);
    
    // Inner glowing dot
    g.setColour(juce::Colours::white.withAlpha(0.8f));
    g.fillEllipse(puck.reduced(10.0f));
  }

  void mouseDown(const juce::MouseEvent& e) override {
    updateVolumes(e.position);
  }
  
  void mouseDrag(const juce::MouseEvent& e) override {
    updateVolumes(e.position);
  }

private:
  void updateVolumes(juce::Point<float> pos) {
    auto bounds = getLocalBounds().toFloat();
    float x = juce::jlimit(0.0f, 1.0f, (pos.x - bounds.getX()) / bounds.getWidth());
    float y = juce::jlimit(0.0f, 1.0f, (pos.y - bounds.getY()) / bounds.getHeight());
    
    float v1 = (1.0f - x) * (1.0f - y);
    float v2 = x * (1.0f - y);
    float v3 = (1.0f - x) * y;
    float v4 = x * y;
    
    audioProcessor.getLayer(0).getParams().volume = v1;
    audioProcessor.getLayer(1).getParams().volume = v2;
    audioProcessor.getLayer(2).getParams().volume = v3;
    audioProcessor.getLayer(3).getParams().volume = v4;
    
    repaint();
  }

  GranularSynthAudioProcessor& audioProcessor;
};

class XYPadUI : public juce::Component, public juce::Timer {
public:
  XYPadUI(GranularSynthAudioProcessor& p) : xyPad(p) {
    addAndMakeVisible(xyPad);
    startTimerHz(30);
  }
  
  void timerCallback() override {
    if (!xyPad.isMouseButtonDown()) {
      xyPad.repaint();
    }
  }

  void paint(juce::Graphics& g) override {
    auto area = getLocalBounds().toFloat().reduced(10);
    juce::ColourGradient bgGrad(
        juce::Colours::white.withAlpha(0.4f), area.getTopLeft(),
        juce::Colours::white.withAlpha(0.15f), area.getBottomRight(), false);
    g.setGradientFill(bgGrad);
    g.fillRoundedRectangle(area, 10.0f);
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(area, 10.0f, 1.5f);
  }
  
  void resized() override {
    xyPad.setBounds(getLocalBounds().reduced(50));
  }

private:
  XYPadComponent xyPad;
};

class CustomTabbedComponent : public juce::TabbedComponent {
public:
  CustomTabbedComponent(juce::TabbedButtonBar::Orientation orientation)
      : juce::TabbedComponent(orientation) {}

  void resized() override {
    auto bounds = getLocalBounds();
    auto &bar = getTabbedButtonBar();

    // Position the tab bar inside the navbar (navbar is y=0 to 70)
    // Leave 240px on left for logo, and 70px on right for about button
    bar.setBounds(240, 15, bounds.getWidth() - 310, 40);

    // Position the content panel below the navbar (y = 85) with 15px side
    // padding
    for (auto *child : getChildren()) {
      if (child != &bar) {
        child->setBounds(bounds.withTop(85).reduced(15, 0));
      }
    }
  }
};

class GranularSynthAudioProcessorEditor : public juce::AudioProcessorEditor,
                                          public juce::Timer {
public:
  GranularSynthAudioProcessorEditor(GranularSynthAudioProcessor &);
  ~GranularSynthAudioProcessorEditor() override;

  void paint(juce::Graphics &) override;
  void resized() override;
  void timerCallback() override;

private:
  GranularSynthAudioProcessor &audioProcessor;
  CustomTabbedComponent tabs;
  CustomKeyboard keyboard;

  SpringSlider pitchWheel;
  juce::Slider modWheel;
  juce::Label pitchLabel, modLabel;

  CustomLookAndFeel customLookAndFeel;
  juce::Image logoImage;
  juce::Image backgroundImage;
  juce::TextButton aboutButton;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
      GranularSynthAudioProcessorEditor)
};
