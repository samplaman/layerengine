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

    g.setColour(isSelected ? juce::Colour(9, 136, 131) : juce::Colours::white.withAlpha(0.85f));
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

  void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                             bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
  {
      if (backgroundColour == juce::Colours::transparentBlack)
      {
          if (shouldDrawButtonAsHighlighted || shouldDrawButtonAsDown)
          {
              auto area = button.getLocalBounds().toFloat();
              g.setColour(juce::Colours::white.withAlpha(shouldDrawButtonAsDown ? 0.2f : 0.1f));
              g.fillRoundedRectangle(area, 6.0f);
          }
          return;
      }
      juce::LookAndFeel_V4::drawButtonBackground(g, button, backgroundColour, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
  }

  juce::Font getTextButtonFont (juce::TextButton& button, int buttonHeight) override
  {
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
    g.setColour(juce::Colours::white.withAlpha(0.95f)); // Light for readability
    g.fillRoundedRectangle(0, 0, (float)width, (float)height, 6.0f);

    g.setColour(juce::Colours::black.withAlpha(0.15f));
    g.drawRoundedRectangle(0.5f, 0.5f, (float)width - 1.0f,
                           (float)height - 1.0f, 6.0f, 1.0f);
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
};

class CustomKeyboard : public juce::MidiKeyboardComponent
{
public:
    CustomKeyboard(juce::MidiKeyboardState& state, juce::MidiKeyboardComponent::Orientation orientation)
        : juce::MidiKeyboardComponent(state, orientation) {}

    void drawWhiteNote(int midiNoteNumber, juce::Graphics &g,
                       juce::Rectangle<float> area, bool isDown, bool isOver,
                       juce::Colour lineColour, juce::Colour textColour) override {
        auto accent = juce::Colour(9, 136, 131);

        juce::Colour fillColour =
            isDown ? accent.withAlpha(0.7f)
                   : (isOver ? juce::Colours::white.withAlpha(0.45f)
                             : juce::Colours::white.withAlpha(0.25f));

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

        juce::Colour fillColour =
            isDown ? accent.withAlpha(0.8f)
                   : (isOver ? juce::Colours::black.withAlpha(0.7f)
                             : juce::Colours::black.withAlpha(0.4f));

        auto keyArea = area.reduced(1.5f, 0.0f).withTrimmedBottom(isDown ? 1.0f : 2.0f);

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
      float posIdx = layer.getParams().position * getWidth();
      g.drawVerticalLine((int)posIdx, 2.0f, (float)getHeight() - 2.0f);
    }
  }

private:
  GranularLayer &layer;
};

class LayerUI : public juce::Component, public juce::Timer, public juce::FileDragAndDropTarget {
public:
  LayerUI(GranularLayer &layer) : layer(layer), viewer(layer) {
    addAndMakeVisible(viewer);

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

    setupSlider(posSlider, "POS", 0.0f, 1.0f, layer.getParams().position, true);
    setupSlider(sizeSlider, "SIZE", 0.01f, 1.0f, layer.getParams().size, true);
    setupSlider(densSlider, "DENS", 1.0f, 100.0f, layer.getParams().density,
                true);
    setupSlider(pitchSlider, "PITCH", 0.1f, 4.0f, layer.getParams().pitch,
                true);

    setupSlider(posRandSlider, "POS JIT", 0.0f, 1.0f,
                layer.getParams().posRandomness, true);
    setupSlider(pitchRandSlider, "PITCH JIT", 0.0f, 1.0f,
                layer.getParams().pitchRandomness, true);
    setupSlider(sizeRandSlider, "SIZE JIT", 0.0f, 1.0f,
                layer.getParams().sizeRandomness, true);

    setupSlider(panSlider, "PAN", -1.0f, 1.0f, layer.getParams().pan, true);
    setupSlider(spreadSlider, "SPREAD", 0.0f, 1.0f, layer.getParams().spread,
                true);

    setupSlider(scanSpeedSlider, "SCAN", -2.0f, 2.0f,
                layer.getParams().scanSpeed, true);
    setupSlider(revProbSlider, "REV %", 0.0f, 1.0f,
                layer.getParams().reverseProbability, true);

    addAndMakeVisible(windowShapeCombo);
    windowShapeCombo.addItem("SINE", 1);
    windowShapeCombo.addItem("HANN", 2);
    windowShapeCombo.addItem("GAUSS", 3);
    windowShapeCombo.addItem("TRI", 4);
    windowShapeCombo.setSelectedId((int)layer.getParams().windowShape + 1);

    setupSlider(cutoffSlider, "FREQ", 20.0f, 20000.0f,
                layer.getParams().filterCutoff, true);
    cutoffSlider.setSkewFactorFromMidPoint(1000.0f);
    setupSlider(resSlider, "RES", 0.1f, 1.0f, layer.getParams().filterResonance,
                true);

    setupSlider(atkSlider, "A", 0.01f, 2.0f, layer.getParams().attack);
    setupSlider(decSlider, "D", 0.01f, 2.0f, layer.getParams().decay);
    setupSlider(susSlider, "S", 0.0f, 1.0f, layer.getParams().sustain);
    setupSlider(relSlider, "R", 0.01f, 5.0f, layer.getParams().release);

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
  bool isInterestedInFileDrag(const juce::StringArray& files) override {
    for (auto& file : files) {
      if (file.endsWithIgnoreCase(".wav") || file.endsWithIgnoreCase(".aif") ||
          file.endsWithIgnoreCase(".aiff") || file.endsWithIgnoreCase(".mp3")) {
        return true;
      }
    }
    return false;
  }

  void fileDragEnter(const juce::StringArray& files, int x, int y) override {
    isDragging = true;
    repaint();
  }

  void fileDragExit(const juce::StringArray& files) override {
    isDragging = false;
    repaint();
  }

  void filesDropped(const juce::StringArray& files, int x, int y) override {
    isDragging = false;
    auto file = juce::File(files[0]);
    if (file.existsAsFile()) {
      layer.loadSample(file);
      sampleNameLabel.setText(file.getFileName().toUpperCase(), juce::dontSendNotification);
      viewer.repaint();
    }
    repaint();
  }

  void timerCallback() override {
    viewer.repaint();
    
    // Dynamic synchronization from processor to UI sliders when not manually dragging
    if (!posSlider.isMouseButtonDown())
      posSlider.setValue(layer.getParams().position, juce::dontSendNotification);
    if (!sizeSlider.isMouseButtonDown())
      sizeSlider.setValue(layer.getParams().size, juce::dontSendNotification);
    if (!densSlider.isMouseButtonDown())
      densSlider.setValue(layer.getParams().density, juce::dontSendNotification);
    if (!pitchSlider.isMouseButtonDown())
      pitchSlider.setValue(layer.getParams().pitch, juce::dontSendNotification);
      
    if (!posRandSlider.isMouseButtonDown())
      posRandSlider.setValue(layer.getParams().posRandomness, juce::dontSendNotification);
    if (!pitchRandSlider.isMouseButtonDown())
      pitchRandSlider.setValue(layer.getParams().pitchRandomness, juce::dontSendNotification);
    if (!sizeRandSlider.isMouseButtonDown())
      sizeRandSlider.setValue(layer.getParams().sizeRandomness, juce::dontSendNotification);
      
    if (!panSlider.isMouseButtonDown())
      panSlider.setValue(layer.getParams().pan, juce::dontSendNotification);
    if (!spreadSlider.isMouseButtonDown())
      spreadSlider.setValue(layer.getParams().spread, juce::dontSendNotification);
      
    if (!scanSpeedSlider.isMouseButtonDown())
      scanSpeedSlider.setValue(layer.getParams().scanSpeed, juce::dontSendNotification);
    if (!revProbSlider.isMouseButtonDown())
      revProbSlider.setValue(layer.getParams().reverseProbability, juce::dontSendNotification);
      
    if (!cutoffSlider.isMouseButtonDown())
      cutoffSlider.setValue(layer.getParams().filterCutoff, juce::dontSendNotification);
    if (!resSlider.isMouseButtonDown())
      resSlider.setValue(layer.getParams().filterResonance, juce::dontSendNotification);
      
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
      windowShapeCombo.setSelectedId(expectedShapeId, juce::dontSendNotification);

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
      g.setColour(juce::Colour(9, 136, 131).withAlpha(0.8f)); // Active glass drag outline
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

    auto coreRow = granArea.removeFromTop(granArea.getHeight() / 2);
    int coreW = coreRow.getWidth() / 6;
    posSlider.setBounds(coreRow.removeFromLeft(coreW).reduced(6));
    sizeSlider.setBounds(coreRow.removeFromLeft(coreW).reduced(6));
    densSlider.setBounds(coreRow.removeFromLeft(coreW).reduced(6));
    pitchSlider.setBounds(coreRow.removeFromLeft(coreW).reduced(6));
    panSlider.setBounds(coreRow.removeFromLeft(coreW).reduced(6));
    spreadSlider.setBounds(coreRow.removeFromLeft(coreW).reduced(6));

    auto jitterRow = granArea;
    int jitW = jitterRow.getWidth() / 6;
    posRandSlider.setBounds(jitterRow.removeFromLeft(jitW).reduced(6));
    pitchRandSlider.setBounds(jitterRow.removeFromLeft(jitW).reduced(6));
    sizeRandSlider.setBounds(jitterRow.removeFromLeft(jitW).reduced(6));
    scanSpeedSlider.setBounds(jitterRow.removeFromLeft(jitW).reduced(6));
    revProbSlider.setBounds(jitterRow.removeFromLeft(jitW).reduced(6));
    windowShapeCombo.setBounds(jitterRow.removeFromLeft(jitW).reduced(12, 20));

    area.removeFromTop(10);

    auto bottomRow = area.removeFromTop(120);

    auto filterArea = bottomRow.removeFromLeft(bottomRow.getWidth() / 3);
    int filterW = filterArea.getWidth() / 2;
    cutoffSlider.setBounds(filterArea.removeFromLeft(filterW).reduced(6));
    resSlider.setBounds(filterArea.removeFromLeft(filterW).reduced(6));

    bottomRow.removeFromLeft(20);

    auto envArea = bottomRow;
    int envW = envArea.getWidth() / 4;
    atkSlider.setBounds(envArea.removeFromLeft(envW).reduced(6));
    decSlider.setBounds(envArea.removeFromLeft(envW).reduced(6));
    susSlider.setBounds(envArea.removeFromLeft(envW).reduced(6));
    relSlider.setBounds(envArea.removeFromLeft(envW).reduced(6));
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
                   float max, float val, bool rotary = false) {
    addAndMakeVisible(s);
    s.setRange(min, max);
    s.setValue(val);
    s.setSliderStyle(rotary ? juce::Slider::RotaryHorizontalVerticalDrag
                            : juce::Slider::LinearVertical);
    s.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    auto *label = labels.add(new juce::Label(name, name));
    addAndMakeVisible(label);
    label->setFont(juce::FontOptions(12.0f).withStyle("Bold"));
    label->setColour(juce::Label::textColourId, juce::Colour(0xff2d3436));
    label->setJustificationType(juce::Justification::centred);
    label->attachToComponent(&s, false);
  }

  GranularLayer &layer;
  WaveformViewer viewer;
  juce::TextButton loadButton;
  juce::TextButton clearButton;
  juce::Label sampleNameLabel;
  juce::Label granLabel, filterLabel, envLabel;
  juce::Slider posSlider, sizeSlider, densSlider, pitchSlider;
  juce::Slider posRandSlider, pitchRandSlider, sizeRandSlider;
  juce::Slider panSlider, spreadSlider;
  juce::Slider scanSpeedSlider, revProbSlider;
  juce::ComboBox windowShapeCombo;
  juce::Slider atkSlider, decSlider, susSlider, relSlider;
  juce::Slider cutoffSlider, resSlider;
  juce::OwnedArray<juce::Label> labels;
  std::unique_ptr<juce::FileChooser> chooser;
  bool isDragging = false;
};

class EffectsUI : public juce::Component, public juce::Timer {
public:
  EffectsUI(GranularSynthAudioProcessor &p) : audioProcessor(p) {
    // 1. Reverb Bypass & Sliders
    addAndMakeVisible(reverbBypassBtn);
    reverbBypassBtn.setClickingTogglesState(true);
    reverbBypassBtn.setToggleState(!audioProcessor.getFXParams().reverbBypass, juce::dontSendNotification);
    reverbBypassBtn.setColour(juce::TextButton::buttonColourId, juce::Colour(45, 52, 54).withAlpha(0.4f));
    reverbBypassBtn.setColour(juce::TextButton::buttonOnColourId, juce::Colour(9, 136, 131).withAlpha(0.85f)); // Soft glowing teal
    reverbBypassBtn.setColour(juce::TextButton::textColourOffId, juce::Colour(120, 120, 120));
    reverbBypassBtn.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    reverbBypassBtn.onClick = [this] {
      audioProcessor.getFXParams().reverbBypass = !reverbBypassBtn.getToggleState();
    };

    setupSlider(reverbSizeSlider, "REV SIZE", audioProcessor.getReverbParams().roomSize);
    reverbSizeSlider.onValueChange = [this] {
      audioProcessor.getReverbParams().roomSize = (float)reverbSizeSlider.getValue();
    };

    setupSlider(reverbWetSlider, "REV WET", audioProcessor.getReverbParams().wetLevel);
    reverbWetSlider.onValueChange = [this] {
      audioProcessor.getReverbParams().wetLevel = (float)reverbWetSlider.getValue();
    };

    // 2. Chorus Bypass & Sliders
    addAndMakeVisible(chorusBypassBtn);
    chorusBypassBtn.setClickingTogglesState(true);
    chorusBypassBtn.setToggleState(!audioProcessor.getFXParams().chorusBypass, juce::dontSendNotification);
    chorusBypassBtn.setColour(juce::TextButton::buttonColourId, juce::Colour(45, 52, 54).withAlpha(0.4f));
    chorusBypassBtn.setColour(juce::TextButton::buttonOnColourId, juce::Colour(9, 136, 131).withAlpha(0.85f));
    chorusBypassBtn.setColour(juce::TextButton::textColourOffId, juce::Colour(120, 120, 120));
    chorusBypassBtn.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    chorusBypassBtn.onClick = [this] {
      audioProcessor.getFXParams().chorusBypass = !chorusBypassBtn.getToggleState();
    };

    setupSlider(chorusRateSlider, "CHO RATE", audioProcessor.getFXParams().chorusRate, 0.1f, 10.0f);
    chorusRateSlider.onValueChange = [this] {
      audioProcessor.getFXParams().chorusRate = (float)chorusRateSlider.getValue();
    };

    setupSlider(chorusDepthSlider, "CHO DEPTH", audioProcessor.getFXParams().chorusDepth, 0.0f, 1.0f);
    chorusDepthSlider.onValueChange = [this] {
      audioProcessor.getFXParams().chorusDepth = (float)chorusDepthSlider.getValue();
    };

    setupSlider(chorusMixSlider, "CHO MIX", audioProcessor.getFXParams().chorusMix, 0.0f, 1.0f);
    chorusMixSlider.onValueChange = [this] {
      audioProcessor.getFXParams().chorusMix = (float)chorusMixSlider.getValue();
    };

    // 3. Filter Bypass & Sliders
    addAndMakeVisible(filterBypassBtn);
    filterBypassBtn.setClickingTogglesState(true);
    filterBypassBtn.setToggleState(!audioProcessor.getFXParams().filterBypass, juce::dontSendNotification);
    filterBypassBtn.setColour(juce::TextButton::buttonColourId, juce::Colour(45, 52, 54).withAlpha(0.4f));
    filterBypassBtn.setColour(juce::TextButton::buttonOnColourId, juce::Colour(9, 136, 131).withAlpha(0.85f));
    filterBypassBtn.setColour(juce::TextButton::textColourOffId, juce::Colour(120, 120, 120));
    filterBypassBtn.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    filterBypassBtn.onClick = [this] {
      audioProcessor.getFXParams().filterBypass = !filterBypassBtn.getToggleState();
    };

    setupSlider(filterCutoffSlider, "MASTER HP/LP", audioProcessor.getFXParams().filterCutoff, 20.0f, 20000.0f);
    filterCutoffSlider.setSkewFactorFromMidPoint(1000.0f);
    filterCutoffSlider.onValueChange = [this] {
      audioProcessor.getFXParams().filterCutoff = (float)filterCutoffSlider.getValue();
    };

    setupSlider(filterResSlider, "MST RES", audioProcessor.getFXParams().filterResonance, 0.1f, 2.0f);
    filterResSlider.onValueChange = [this] {
      audioProcessor.getFXParams().filterResonance = (float)filterResSlider.getValue();
    };

    // 4. Limiter Bypass & Sliders
    addAndMakeVisible(limiterBypassBtn);
    limiterBypassBtn.setClickingTogglesState(true);
    limiterBypassBtn.setToggleState(!audioProcessor.getFXParams().limiterBypass, juce::dontSendNotification);
    limiterBypassBtn.setColour(juce::TextButton::buttonColourId, juce::Colour(45, 52, 54).withAlpha(0.4f));
    limiterBypassBtn.setColour(juce::TextButton::buttonOnColourId, juce::Colour(9, 136, 131).withAlpha(0.85f));
    limiterBypassBtn.setColour(juce::TextButton::textColourOffId, juce::Colour(120, 120, 120));
    limiterBypassBtn.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    limiterBypassBtn.onClick = [this] {
      audioProcessor.getFXParams().limiterBypass = !limiterBypassBtn.getToggleState();
    };

    setupSlider(limiterThresholdSlider, "LMT THRESH", audioProcessor.getFXParams().limiterThreshold, -40.0f, 0.0f);
    limiterThresholdSlider.onValueChange = [this] {
      audioProcessor.getFXParams().limiterThreshold = (float)limiterThresholdSlider.getValue();
    };

    setupSlider(limiterReleaseSlider, "LMT REL", audioProcessor.getFXParams().limiterRelease, 1.0f, 500.0f);
    limiterReleaseSlider.onValueChange = [this] {
      audioProcessor.getFXParams().limiterRelease = (float)limiterReleaseSlider.getValue();
    };
    
    startTimerHz(30);
  }

  void timerCallback() override {
    if (!reverbSizeSlider.isMouseButtonDown())
      reverbSizeSlider.setValue(audioProcessor.getReverbParams().roomSize, juce::dontSendNotification);
    if (!reverbWetSlider.isMouseButtonDown())
      reverbWetSlider.setValue(audioProcessor.getReverbParams().wetLevel, juce::dontSendNotification);
      
    if (!chorusRateSlider.isMouseButtonDown())
      chorusRateSlider.setValue(audioProcessor.getFXParams().chorusRate, juce::dontSendNotification);
    if (!chorusDepthSlider.isMouseButtonDown())
      chorusDepthSlider.setValue(audioProcessor.getFXParams().chorusDepth, juce::dontSendNotification);
    if (!chorusMixSlider.isMouseButtonDown())
      chorusMixSlider.setValue(audioProcessor.getFXParams().chorusMix, juce::dontSendNotification);
      
    if (!filterCutoffSlider.isMouseButtonDown())
      filterCutoffSlider.setValue(audioProcessor.getFXParams().filterCutoff, juce::dontSendNotification);
    if (!filterResSlider.isMouseButtonDown())
      filterResSlider.setValue(audioProcessor.getFXParams().filterResonance, juce::dontSendNotification);
      
    if (!limiterThresholdSlider.isMouseButtonDown())
      limiterThresholdSlider.setValue(audioProcessor.getFXParams().limiterThreshold, juce::dontSendNotification);
    if (!limiterReleaseSlider.isMouseButtonDown())
      limiterReleaseSlider.setValue(audioProcessor.getFXParams().limiterRelease, juce::dontSendNotification);

    // Sync bypass button states
    bool expectedReverbToggle = !audioProcessor.getFXParams().reverbBypass;
    if (reverbBypassBtn.getToggleState() != expectedReverbToggle)
      reverbBypassBtn.setToggleState(expectedReverbToggle, juce::dontSendNotification);

    bool expectedChorusToggle = !audioProcessor.getFXParams().chorusBypass;
    if (chorusBypassBtn.getToggleState() != expectedChorusToggle)
      chorusBypassBtn.setToggleState(expectedChorusToggle, juce::dontSendNotification);

    bool expectedFilterToggle = !audioProcessor.getFXParams().filterBypass;
    if (filterBypassBtn.getToggleState() != expectedFilterToggle)
      filterBypassBtn.setToggleState(expectedFilterToggle, juce::dontSendNotification);

    bool expectedLimiterToggle = !audioProcessor.getFXParams().limiterBypass;
    if (limiterBypassBtn.getToggleState() != expectedLimiterToggle)
      limiterBypassBtn.setToggleState(expectedLimiterToggle, juce::dontSendNotification);
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
  }

  void resized() override {
    auto area = getLocalBounds().reduced(40, 20);

    // Four rows, each row has a bypass button on the left
    auto reverbArea = area.removeFromTop(area.getHeight() / 4);
    auto reverbBtnArea = reverbArea.removeFromLeft(70).reduced(0, 20);
    reverbBypassBtn.setBounds(reverbBtnArea);
    int revW = reverbArea.getWidth() / 2;
    reverbSizeSlider.setBounds(reverbArea.removeFromLeft(revW).reduced(20, 10));
    reverbWetSlider.setBounds(reverbArea.removeFromLeft(revW).reduced(20, 10));

    auto chorusArea = area.removeFromTop(area.getHeight() / 3);
    auto chorusBtnArea = chorusArea.removeFromLeft(70).reduced(0, 20);
    chorusBypassBtn.setBounds(chorusBtnArea);
    int choW = chorusArea.getWidth() / 3;
    chorusRateSlider.setBounds(chorusArea.removeFromLeft(choW).reduced(20, 10));
    chorusDepthSlider.setBounds(chorusArea.removeFromLeft(choW).reduced(20, 10));
    chorusMixSlider.setBounds(chorusArea.removeFromLeft(choW).reduced(20, 10));

    auto filterArea = area.removeFromTop(area.getHeight() / 2);
    auto filterBtnArea = filterArea.removeFromLeft(70).reduced(0, 20);
    filterBypassBtn.setBounds(filterBtnArea);
    int filW = filterArea.getWidth() / 2;
    filterCutoffSlider.setBounds(filterArea.removeFromLeft(filW).reduced(20, 10));
    filterResSlider.setBounds(filterArea.removeFromLeft(filW).reduced(20, 10));

    auto limiterArea = area;
    auto limiterBtnArea = limiterArea.removeFromLeft(70).reduced(0, 20);
    limiterBypassBtn.setBounds(limiterBtnArea);
    int limW = limiterArea.getWidth() / 2;
    limiterThresholdSlider.setBounds(limiterArea.removeFromLeft(limW).reduced(20, 10));
    limiterReleaseSlider.setBounds(limiterArea.removeFromLeft(limW).reduced(20, 10));
  }

private:
  void setupSlider(juce::Slider &s, const juce::String &name, float val, float min = 0.0f, float max = 1.0f) {
    addAndMakeVisible(s);
    s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    s.setRange(min, max);
    s.setValue(val);
    s.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    auto *label = labels.add(new juce::Label(name, name));
    addAndMakeVisible(label);
    label->setFont(juce::FontOptions(12.0f).withStyle("Bold"));
    label->setColour(juce::Label::textColourId,
                     juce::Colour(0xff2d3436).withAlpha(0.7f));
    label->setJustificationType(juce::Justification::centred);
    label->attachToComponent(&s, false);
  }

  GranularSynthAudioProcessor &audioProcessor;
  juce::TextButton reverbBypassBtn {"ON"}, chorusBypassBtn {"ON"}, filterBypassBtn {"ON"}, limiterBypassBtn {"ON"};
  juce::Slider reverbSizeSlider, reverbWetSlider;
  juce::Slider chorusRateSlider, chorusDepthSlider, chorusMixSlider;
  juce::Slider filterCutoffSlider, filterResSlider;
  juce::Slider limiterThresholdSlider, limiterReleaseSlider;
  juce::OwnedArray<juce::Label> labels;
};

class MixerUI : public juce::Component, public juce::Timer {
public:
  MixerUI(GranularSynthAudioProcessor &p) : audioProcessor(p) {
    for (int i = 0; i < 4; ++i) {
      // 1. Volume Sliders
      auto *s = sliders.add(new juce::Slider());
      addAndMakeVisible(s);
      s->setSliderStyle(juce::Slider::LinearVertical);
      s->setRange(0.0f, 1.0f);
      s->setValue(audioProcessor.getLayer(i).getParams().volume);
      s->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
      s->onValueChange = [this, i, s] {
        audioProcessor.getLayer(i).getParams().volume = (float)s->getValue();
      };

      // 2. Pan Sliders
      auto *ps = panSliders.add(new juce::Slider());
      addAndMakeVisible(ps);
      ps->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
      ps->setRange(-1.0f, 1.0f);
      ps->setValue(audioProcessor.getLayer(i).getParams().pan);
      ps->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
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
      auto *pl = panLabels.add(new juce::Label("PL" + juce::String(i + 1), "PAN"));
      addAndMakeVisible(pl);
      pl->setFont(juce::FontOptions(10.0f).withStyle("Bold"));
      pl->setColour(juce::Label::textColourId, juce::Colour(0xff2d3436).withAlpha(0.6f));
      pl->setJustificationType(juce::Justification::centred);

      // 5. Mute Buttons
      auto *m = muteButtons.add(new juce::TextButton("M"));
      addAndMakeVisible(m);
      m->setClickingTogglesState(true);
      m->setToggleState(audioProcessor.getLayer(i).getParams().isMuted, juce::dontSendNotification);
      m->setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
      m->setColour(juce::TextButton::buttonOnColourId, juce::Colour(220, 80, 80).withAlpha(0.8f)); // Clean soft red
      m->setColour(juce::TextButton::textColourOffId, juce::Colour(0xff2d3436));
      m->setColour(juce::TextButton::textColourOnId, juce::Colours::white);
      m->onClick = [this, i, m] {
        audioProcessor.getLayer(i).getParams().isMuted = m->getToggleState();
      };

      // 6. Solo Buttons
      auto *sol = soloButtons.add(new juce::TextButton("S"));
      addAndMakeVisible(sol);
      sol->setClickingTogglesState(true);
      sol->setToggleState(audioProcessor.getLayer(i).getParams().isSoloed, juce::dontSendNotification);
      sol->setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
      sol->setColour(juce::TextButton::buttonOnColourId, juce::Colour(230, 160, 40).withAlpha(0.8f)); // Clean soft amber
      sol->setColour(juce::TextButton::textColourOffId, juce::Colour(0xff2d3436));
      sol->setColour(juce::TextButton::textColourOnId, juce::Colours::white);
      sol->onClick = [this, i, sol] {
        audioProcessor.getLayer(i).getParams().isSoloed = sol->getToggleState();
      };
    }
    
    startTimerHz(30);
  }

  void timerCallback() override {
    for (int i = 0; i < 4; ++i) {
      if (i < sliders.size() && sliders[i] != nullptr && !sliders[i]->isMouseButtonDown()) {
        sliders[i]->setValue(audioProcessor.getLayer(i).getParams().volume, juce::dontSendNotification);
      }
      if (i < panSliders.size() && panSliders[i] != nullptr && !panSliders[i]->isMouseButtonDown()) {
        panSliders[i]->setValue(audioProcessor.getLayer(i).getParams().pan, juce::dontSendNotification);
      }
      if (i < muteButtons.size() && muteButtons[i] != nullptr) {
        muteButtons[i]->setToggleState(audioProcessor.getLayer(i).getParams().isMuted, juce::dontSendNotification);
      }
      if (i < soloButtons.size() && soloButtons[i] != nullptr) {
        soloButtons[i]->setToggleState(audioProcessor.getLayer(i).getParams().isSoloed, juce::dontSendNotification);
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
      faderArea.removeFromTop(80);    // Remove title label, pan knob, and pan label
      
      int meterY = faderArea.getY() + 10;
      int meterH = faderArea.getHeight() - 20;

      // Draw meter track background
      g.setColour(juce::Colours::black.withAlpha(0.12f));
      g.fillRoundedRectangle((float)meterX, (float)meterY, (float)meterW, (float)meterH, 3.0f);

      // Track outline
      g.setColour(juce::Colours::white.withAlpha(0.2f));
      g.drawRoundedRectangle((float)meterX, (float)meterY, (float)meterW, (float)meterH, 3.0f, 1.0f);

      // Smooth decay tracking
      float rawLevel = audioProcessor.getLayer(i).getCurrentLevel();
      float currentVisual = visualLevels[i];
      if (rawLevel > currentVisual) {
        currentVisual = rawLevel;
      } else {
        currentVisual -= 0.08f; // Smooth linear decay
        if (currentVisual < 0.0f) currentVisual = 0.0f;
      }
      visualLevels[i] = currentVisual;

      float levelHeight = currentVisual * meterH;
      if (levelHeight > meterH) levelHeight = (float)meterH;

      if (levelHeight > 0.0f) {
        // Glowing teal HSL gradient
        juce::Colour accentColor = juce::Colour(9, 136, 131);
        juce::ColourGradient meterGrad(
            accentColor.withAlpha(0.85f), (float)meterX, (float)(meterY + meterH),
            accentColor.brighter(0.4f).withAlpha(0.95f), (float)meterX, (float)(meterY + meterH - levelHeight),
            false);
        g.setGradientFill(meterGrad);
        g.fillRoundedRectangle(area.getIntersection(juce::Rectangle<float>((float)meterX, (float)(meterY + meterH - levelHeight), (float)meterW, levelHeight)), 3.0f);

        // Glow aura
        g.setColour(accentColor.withAlpha(0.2f * currentVisual));
        g.drawRoundedRectangle((float)meterX - 1.5f, (float)(meterY + meterH - levelHeight) - 1.5f, (float)meterW + 3.0f, levelHeight + 3.0f, 3.0f, 1.5f);
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
      auto leftBtn = buttonArea.removeFromLeft(buttonArea.getWidth() / 2).reduced(2, 0);
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
  juce::OwnedArray<juce::Slider> sliders;
  juce::OwnedArray<juce::Slider> panSliders;
  juce::OwnedArray<juce::Label> labels;
  juce::OwnedArray<juce::Label> panLabels;
  juce::OwnedArray<juce::TextButton> muteButtons;
  juce::OwnedArray<juce::TextButton> soloButtons;
  float visualLevels[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
};

class CustomTabbedComponent : public juce::TabbedComponent
{
public:
    CustomTabbedComponent(juce::TabbedButtonBar::Orientation orientation)
        : juce::TabbedComponent(orientation) {}

    void resized() override
    {
        auto bounds = getLocalBounds();
        auto& bar = getTabbedButtonBar();
        
        // Position the tab bar inside the navbar (navbar is y=0 to 70)
        // Leave 240px on left for logo, and 70px on right for about button
        bar.setBounds(240, 15, bounds.getWidth() - 310, 40);
        
        // Position the content panel below the navbar (y = 85) with 15px side padding
        for (auto* child : getChildren())
        {
            if (child != &bar)
            {
                child->setBounds(bounds.withTop(85).reduced(15, 0));
            }
        }
    }
};

class GranularSynthAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer {
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
