#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

// Glassmorphism Light Theme
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        auto accentWhite = juce::Colours::white;
        auto lightGrey = juce::Colour(0xffcccccc);
        
        setColour(juce::Slider::thumbColourId, accentWhite);
        setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white.withAlpha(0.2f));
        setColour(juce::Slider::rotarySliderFillColourId, accentWhite.withAlpha(0.8f));
        setColour(juce::Slider::trackColourId, juce::Colours::white.withAlpha(0.1f));
        
        setColour(juce::TextButton::buttonColourId, juce::Colours::white.withAlpha(0.1f));
        setColour(juce::TextButton::buttonOnColourId, accentWhite.withAlpha(0.3f));
        setColour(juce::TextButton::textColourOffId, lightGrey);
        setColour(juce::TextButton::textColourOnId, juce::Colours::white);
        
        setColour(juce::TabbedButtonBar::tabTextColourId, juce::Colours::white);
        setColour(juce::TabbedButtonBar::tabOutlineColourId, juce::Colours::transparentBlack);
        setColour(juce::TabbedComponent::outlineColourId, juce::Colours::transparentBlack);
        
        setColour(juce::Label::textColourId, juce::Colours::white);

        // Keyboard Styling
        setColour(juce::MidiKeyboardComponent::whiteNoteColourId, juce::Colours::white.withAlpha(0.9f));
        setColour(juce::MidiKeyboardComponent::blackNoteColourId, juce::Colour(0xff1a1a1a));
        setColour(juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId, accentWhite.withAlpha(0.2f));
        setColour(juce::MidiKeyboardComponent::keyDownOverlayColourId, accentWhite.withAlpha(0.5f));
    }

    void drawTabButton (juce::TabBarButton& button, juce::Graphics& g, bool isMouseOver, bool isMouseDown) override
    {
        auto area = button.getLocalBounds().toFloat().reduced(2, 2);
        bool isSelected = button.isFrontTab();
        
        if (isSelected || isMouseOver)
        {
            g.setColour(juce::Colours::white.withAlpha(isSelected ? 0.15f : 0.08f));
            g.fillRoundedRectangle(area, 6.0f);
            
            g.setColour(juce::Colours::white.withAlpha(isSelected ? 0.4f : 0.2f));
            g.drawRoundedRectangle(area, 6.0f, 1.0f);
        }

        g.setColour(isSelected ? juce::Colours::white : juce::Colours::white.withAlpha(0.6f));
        g.setFont(juce::FontOptions(13.0f).withStyle(isSelected ? "Bold" : "Plain"));
        g.drawText(button.getButtonText(), area, juce::Justification::centred);
    }

    void drawTabbedButtonBarBackground(juce::TabbedButtonBar&, juce::Graphics& g) override
    {
        // Keep it transparent
    }

    void drawTabAreaBehindFrontButton (juce::TabbedButtonBar&, juce::Graphics&, int, int) override
    {
        // Remove shadow under tab header
    }

    void drawComboBox (juce::Graphics& g, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& box) override
    {
        auto area = juce::Rectangle<int>(0, 0, width, height).toFloat().reduced(2);
        
        g.setColour(juce::Colours::white.withAlpha(0.1f));
        g.fillRoundedRectangle(area, 4.0f);
        
        g.setColour(juce::Colours::white.withAlpha(0.2f));
        g.drawRoundedRectangle(area, 4.0f, 1.0f);
        
        // Draw the little arrow
        juce::Path p;
        float arrowSize = 0.25f * (float)juce::jmin(buttonW, buttonH);
        p.addTriangle((float)buttonX + (float)buttonW * 0.5f - arrowSize, (float)buttonY + (float)buttonH * 0.5f - arrowSize * 0.5f,
                      (float)buttonX + (float)buttonW * 0.5f + arrowSize, (float)buttonY + (float)buttonH * 0.5f - arrowSize * 0.5f,
                      (float)buttonX + (float)buttonW * 0.5f, (float)buttonY + (float)buttonH * 0.5f + arrowSize * 0.5f);
        
        g.setColour(juce::Colours::white.withAlpha(0.6f));
        g.fillPath(p);
    }

    void drawPopupMenuBackground (juce::Graphics& g, int width, int height) override
    {
        g.setColour(juce::Colour(0xff2d3436).withAlpha(0.95f)); // Darker for readability
        g.fillRoundedRectangle(0, 0, (float)width, (float)height, 6.0f);
        
        g.setColour(juce::Colours::white.withAlpha(0.15f));
        g.drawRoundedRectangle(0.5f, 0.5f, (float)width - 1.0f, (float)height - 1.0f, 6.0f, 1.0f);
    }

    void drawPopupMenuItem (juce::Graphics& g, const juce::Rectangle<int>& area,
                           bool isSeparator, bool isActive, bool isHighlighted, bool isChecked, bool isEnabled,
                           const juce::String& text, const juce::String& shortcutKeyText,
                           const juce::Drawable* icon, const juce::Colour* textColourToUse) override
    {
        if (isHighlighted && isEnabled)
        {
            g.setColour(juce::Colours::white.withAlpha(0.15f));
            g.fillRect(area.reduced(2));
        }

        g.setColour(isEnabled ? juce::Colours::white : juce::Colours::white.withAlpha(0.3f));
        g.setFont(juce::FontOptions(13.0f));

        auto r = area.reduced(10, 0);
        g.drawText(text, r, juce::Justification::centredLeft, true);
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
    {
        auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(10);
        auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryStartAngle - rotaryEndAngle); // Wait, fix direction?
        // Actually toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = 4.5f; // Chunky!
        auto arcRadius = radius - lineW * 0.5f;

        // Background Arc
        juce::Path backgroundArc;
        backgroundArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
        g.setColour(juce::Colours::white.withAlpha(0.1f));
        g.strokePath(backgroundArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        if (slider.isEnabled())
        {
            // Chunky Value Arc
            juce::Path valueArc;
            valueArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius, 0.0f, rotaryStartAngle, toAngle, true);
            g.setColour(juce::Colours::white.withAlpha(0.85f));
            g.strokePath(valueArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

            // Chunky Pointer
            g.setColour(juce::Colours::white);
            auto pointerLength = radius * 0.7f;
            juce::Path needle;
            needle.startNewSubPath(bounds.getCentreX(), bounds.getCentreY());
            needle.lineTo(bounds.getCentreX() + pointerLength * std::cos(toAngle - juce::MathConstants<float>::halfPi),
                          bounds.getCentreY() + pointerLength * std::sin(toAngle - juce::MathConstants<float>::halfPi));
            g.strokePath(needle, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }
    }
};

class SpringSlider : public juce::Slider
{
public:
    SpringSlider() : juce::Slider() {}
    void mouseUp(const juce::MouseEvent& e) override
    {
        juce::Slider::mouseUp(e);
        setValue(0.0, juce::sendNotificationAsync);
    }
};

class WaveformViewer : public juce::Component
{
public:
    WaveformViewer(GranularLayer& l) : layer(l) {}

    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();
        // Transparent background
        
        auto& buffer = layer.getSampleBuffer();
        if (buffer.getNumSamples() > 0)
        {
            g.setColour(juce::Colours::white.withAlpha(0.6f));
            auto ratio = (float)buffer.getNumSamples() / (float)getWidth();
            
            juce::Path p;
            p.startNewSubPath(0, getHeight() / 2.0f);
            
            for (int x = 0; x < getWidth(); ++x)
            {
                int sampleIdx = (int)(x * ratio);
                float val = buffer.getSample(0, sampleIdx);
                p.lineTo((float)x, (getHeight() / 2.0f) + (val * getHeight() / 2.2f));
            }
            g.strokePath(p, juce::PathStrokeType(1.5f));

            for (const auto& grain : layer.getGrains())
            {
                if (grain.active)
                {
                    float xCurrent = (float)(grain.startSample + grain.currentOffset) / buffer.getNumSamples() * getWidth();
                    float alpha = std::sin(((float)grain.currentOffset / grain.length) * juce::MathConstants<float>::pi);
                    g.setColour(juce::Colours::white.withAlpha(alpha * 0.9f));
                    g.drawVerticalLine((int)xCurrent, 5.0f, (float)getHeight() - 5.0f);
                }
            }

            g.setColour(juce::Colour(0xff2d3436).withAlpha(0.4f));
            float posIdx = layer.getParams().position * getWidth();
            g.drawVerticalLine((int)posIdx, 2.0f, (float)getHeight() - 2.0f);
        }
    }

private:
    GranularLayer& layer;
};

class LayerUI : public juce::Component, public juce::Timer
{
public:
    LayerUI(GranularLayer& layer) : layer(layer), viewer(layer)
    {
        addAndMakeVisible(viewer);

        addAndMakeVisible(loadButton);
        loadButton.setButtonText("IMPORT AUDIO");
        loadButton.onClick = [this] {
            chooser = std::make_unique<juce::FileChooser>("Select a sample", juce::File::getSpecialLocation(juce::File::userHomeDirectory), "*.wav;*.aif;*.mp3");
            chooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles, [this](const juce::FileChooser& fc) {
                auto file = fc.getResult();
                if (file.existsAsFile()) {
                    this->layer.loadSample(file);
                    sampleNameLabel.setText(file.getFileName().toUpperCase(), juce::dontSendNotification);
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
        sampleNameLabel.setColour(juce::Label::textColourId, juce::Colour(0xff2d3436));

        setupSlider(posSlider, "POS", 0.0f, 1.0f, layer.getParams().position, true);
        setupSlider(sizeSlider, "SIZE", 0.01f, 1.0f, layer.getParams().size, true);
        setupSlider(densSlider, "DENS", 1.0f, 100.0f, layer.getParams().density, true);
        setupSlider(pitchSlider, "PITCH", 0.1f, 4.0f, layer.getParams().pitch, true);
        
        setupSlider(posRandSlider, "POS JIT", 0.0f, 1.0f, layer.getParams().posRandomness, true);
        setupSlider(pitchRandSlider, "PITCH JIT", 0.0f, 1.0f, layer.getParams().pitchRandomness, true);
        setupSlider(sizeRandSlider, "SIZE JIT", 0.0f, 1.0f, layer.getParams().sizeRandomness, true);

        setupSlider(panSlider, "PAN", -1.0f, 1.0f, layer.getParams().pan, true);
        setupSlider(spreadSlider, "SPREAD", 0.0f, 1.0f, layer.getParams().spread, true);
        
        setupSlider(scanSpeedSlider, "SCAN", -2.0f, 2.0f, layer.getParams().scanSpeed, true);
        setupSlider(revProbSlider, "REV %", 0.0f, 1.0f, layer.getParams().reverseProbability, true);

        addAndMakeVisible(windowShapeCombo);
        windowShapeCombo.addItem("SINE", 1);
        windowShapeCombo.addItem("HANN", 2);
        windowShapeCombo.addItem("GAUSS", 3);
        windowShapeCombo.addItem("TRI", 4);
        windowShapeCombo.setSelectedId((int)layer.getParams().windowShape + 1);

        setupSlider(cutoffSlider, "FREQ", 20.0f, 20000.0f, layer.getParams().filterCutoff, true);
        cutoffSlider.setSkewFactorFromMidPoint(1000.0f);
        setupSlider(resSlider, "RES", 0.1f, 1.0f, layer.getParams().filterResonance, true);

        setupSlider(atkSlider, "A", 0.01f, 2.0f, layer.getParams().attack);
        setupSlider(decSlider, "D", 0.01f, 2.0f, layer.getParams().decay);
        setupSlider(susSlider, "S", 0.0f, 1.0f, layer.getParams().sustain);
        setupSlider(relSlider, "R", 0.01f, 5.0f, layer.getParams().release);
        
        posSlider.onValueChange = [this] { this->layer.getParams().position = (float)posSlider.getValue(); };
        sizeSlider.onValueChange = [this] { this->layer.getParams().size = (float)sizeSlider.getValue(); };
        densSlider.onValueChange = [this] { this->layer.getParams().density = (float)densSlider.getValue(); };
        pitchSlider.onValueChange = [this] { this->layer.getParams().pitch = (float)pitchSlider.getValue(); };
        
        posRandSlider.onValueChange = [this] { this->layer.getParams().posRandomness = (float)posRandSlider.getValue(); };
        pitchRandSlider.onValueChange = [this] { this->layer.getParams().pitchRandomness = (float)pitchRandSlider.getValue(); };
        sizeRandSlider.onValueChange = [this] { this->layer.getParams().sizeRandomness = (float)sizeRandSlider.getValue(); };

        panSlider.onValueChange = [this] { this->layer.getParams().pan = (float)panSlider.getValue(); };
        spreadSlider.onValueChange = [this] { this->layer.getParams().spread = (float)spreadSlider.getValue(); };
        
        scanSpeedSlider.onValueChange = [this] { this->layer.getParams().scanSpeed = (float)scanSpeedSlider.getValue(); };
        revProbSlider.onValueChange = [this] { this->layer.getParams().reverseProbability = (float)revProbSlider.getValue(); };

        windowShapeCombo.onChange = [this] { this->layer.getParams().windowShape = (WindowShape)(windowShapeCombo.getSelectedId() - 1); };

        cutoffSlider.onValueChange = [this] { this->layer.getParams().filterCutoff = (float)cutoffSlider.getValue(); };
        resSlider.onValueChange = [this] { this->layer.getParams().filterResonance = (float)resSlider.getValue(); };

        atkSlider.onValueChange = [this] { this->layer.getParams().attack = (float)atkSlider.getValue(); };
        decSlider.onValueChange = [this] { this->layer.getParams().decay = (float)decSlider.getValue(); };
        susSlider.onValueChange = [this] { this->layer.getParams().sustain = (float)susSlider.getValue(); };
        relSlider.onValueChange = [this] { this->layer.getParams().release = (float)relSlider.getValue(); };

        startTimerHz(60);
    }

    void timerCallback() override 
    { 
        viewer.repaint(); 
        if (!posSlider.isMouseButtonDown())
            posSlider.setValue(layer.getParams().position, juce::dontSendNotification);
    }

    void paint(juce::Graphics& g) override
    {
        auto area = getLocalBounds().toFloat().reduced(5);
        g.setColour(juce::Colours::white.withAlpha(0.07f));
        g.fillRoundedRectangle(area, 10.0f);
        
        g.setColour(juce::Colours::white.withAlpha(0.15f));
        g.drawRoundedRectangle(area, 10.0f, 1.5f);
    }

    void resized() override
    {
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
    void setupSectionLabel(juce::Label& l, const juce::String& text)
    {
        addAndMakeVisible(l);
        l.setText(text, juce::dontSendNotification);
        l.setFont(juce::FontOptions(15.0f).withStyle("Bold"));
        l.setColour(juce::Label::textColourId, juce::Colours::white);
        l.setJustificationType(juce::Justification::left);
    }

    void setupSlider(juce::Slider& s, const juce::String& name, float min, float max, float val, bool rotary = false)
    {
        addAndMakeVisible(s);
        s.setRange(min, max);
        s.setValue(val);
        s.setSliderStyle(rotary ? juce::Slider::RotaryHorizontalVerticalDrag : juce::Slider::LinearVertical);
        s.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        
        auto* label = labels.add(new juce::Label(name, name));
        addAndMakeVisible(label);
        label->setFont(juce::FontOptions(12.0f).withStyle("Bold"));
        label->setColour(juce::Label::textColourId, juce::Colours::white);
        label->setJustificationType(juce::Justification::centred);
        label->attachToComponent(&s, false);
    }

    GranularLayer& layer;
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
};

class EffectsUI : public juce::Component
{
public:
    EffectsUI(GranularSynthAudioProcessor& p) : audioProcessor(p)
    {
        setupSlider(reverbSizeSlider, "SIZE", audioProcessor.getReverbParams().roomSize);
        reverbSizeSlider.onValueChange = [this] { audioProcessor.getReverbParams().roomSize = (float)reverbSizeSlider.getValue(); };

        setupSlider(reverbWetSlider, "WET", audioProcessor.getReverbParams().wetLevel);
        reverbWetSlider.onValueChange = [this] { audioProcessor.getReverbParams().wetLevel = (float)reverbWetSlider.getValue(); };
    }

    void paint(juce::Graphics& g) override
    {
        auto area = getLocalBounds().toFloat().reduced(10);
        g.setColour(juce::Colours::white.withAlpha(0.07f));
        g.fillRoundedRectangle(area, 10.0f);
        
        g.setColour(juce::Colours::white.withAlpha(0.15f));
        g.drawRoundedRectangle(area, 10.0f, 1.5f);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(40);
        
        int sliderW = area.getWidth() / 2;
        reverbSizeSlider.setBounds(area.removeFromLeft(sliderW).reduced(20));
        reverbWetSlider.setBounds(area.removeFromLeft(sliderW).reduced(20));
    }

private:
    void setupSlider(juce::Slider& s, const juce::String& name, float val)
    {
        addAndMakeVisible(s);
        s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        s.setRange(0.0f, 1.0f);
        s.setValue(val);
        s.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        
        auto* label = labels.add(new juce::Label(name, name));
        addAndMakeVisible(label);
        label->setFont(juce::FontOptions(14.0f).withStyle("Bold"));
        label->setColour(juce::Label::textColourId, juce::Colour(0xff2d3436).withAlpha(0.7f));
        label->setJustificationType(juce::Justification::centred);
        label->attachToComponent(&s, false);
    }

    GranularSynthAudioProcessor& audioProcessor;
    juce::Slider reverbSizeSlider, reverbWetSlider;
    juce::Label reverbLabel;
    juce::OwnedArray<juce::Label> labels;
};

class MixerUI : public juce::Component
{
public:
    MixerUI(GranularSynthAudioProcessor& p) : audioProcessor(p)
    {
        for (int i = 0; i < 4; ++i)
        {
            auto* s = sliders.add(new juce::Slider());
            addAndMakeVisible(s);
            s->setSliderStyle(juce::Slider::LinearVertical);
            s->setRange(0.0f, 1.0f);
            s->setValue(audioProcessor.getLayer(i).getParams().volume);
            s->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
            s->onValueChange = [this, i, s] { audioProcessor.getLayer(i).getParams().volume = (float)s->getValue(); };

            auto* l = labels.add(new juce::Label("L" + juce::String(i + 1), "LAYER " + juce::String(i + 1)));
            addAndMakeVisible(l);
            l->setFont(juce::FontOptions(14.0f).withStyle("Bold"));
            l->setColour(juce::Label::textColourId, juce::Colours::white);
            l->setJustificationType(juce::Justification::centred);
            l->attachToComponent(s, false);
        }
    }

    void paint(juce::Graphics& g) override
    {
        auto area = getLocalBounds().toFloat().reduced(10);
        g.setColour(juce::Colours::white.withAlpha(0.07f));
        g.fillRoundedRectangle(area, 10.0f);
        
        g.setColour(juce::Colours::white.withAlpha(0.15f));
        g.drawRoundedRectangle(area, 10.0f, 1.5f);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(50);
        int sliderW = area.getWidth() / 4;
        
        for (int i = 0; i < 4; ++i)
        {
            sliders[i]->setBounds(area.removeFromLeft(sliderW).reduced(20, 10));
        }
    }

private:
    GranularSynthAudioProcessor& audioProcessor;
    juce::OwnedArray<juce::Slider> sliders;
    juce::OwnedArray<juce::Label> labels;
};

class GranularSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    GranularSynthAudioProcessorEditor (GranularSynthAudioProcessor&);
    ~GranularSynthAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    GranularSynthAudioProcessor& audioProcessor;
    juce::TabbedComponent tabs;
    juce::MidiKeyboardComponent keyboard;
    
    SpringSlider pitchWheel;
    juce::Slider modWheel;
    juce::Label pitchLabel, modLabel;

    CustomLookAndFeel customLookAndFeel;
    juce::Image logoImage;
    juce::Image backgroundImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranularSynthAudioProcessorEditor)
};
