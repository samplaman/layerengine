cmake_minimum_required(VERSION 3.15)

project(GranularSynth VERSION 0.0.1)

# Set JUCE_DIR if you have JUCE installed locally, 
# otherwise we'll fetch it.
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        origin/master 
)
FetchContent_MakeAvailable(JUCE)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PLUGIN_FORMATS ${PLUGIN_FORMATS})
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(PLUGIN_FORMATS AU VST3 Standalone)
else()
    set(PLUGIN_FORMATS VST3 Standalone)
endif()

juce_add_plugin(LayerEngine
    COMPANY_NAME "Eoin O Dowd"
    BUNDLE_ID "com.eoinodowd.layerengine"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_CODE "GRAN"
    FORMATS ${PLUGIN_FORMATS}
    ICON_BIG assets/icon.png
    ICON_SMALL assets/icon.png
)

juce_add_binary_data(Assets
    SOURCES
        assets/logo.png
        assets/mountains.png
)

target_sources(LayerEngine PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/GranularLayer.cpp
    Source/GranularLayer.h
)

target_compile_definitions(LayerEngine PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DIRECT2D=0
    JUCE_USE_DIRECTWRITE=0
)

target_link_libraries(LayerEngine PRIVATE
    Assets
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_gui_extra
    juce::juce_gui_basics
    juce::juce_graphics
    juce::juce_events
    juce::juce_core
    juce::juce_audio_formats
    juce::juce_audio_devices
    juce::juce_audio_basics
    juce::juce_dsp
)

juce_generate_juce_header(LayerEngine)
