cmake_minimum_required(VERSION 3.22)

project(RGain VERSION 0.2.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(external/juce)
add_subdirectory(external/ranze-plugin-ui)

if(APPLE)
    set(RGAIN_FORMATS VST3 AU)
else()
    set(RGAIN_FORMATS VST3)
endif()

juce_add_plugin(RGain
    COMPANY_NAME "Ranze"
    BUNDLE_ID "com.ranze.rgain"
    PLUGIN_MANUFACTURER_CODE Rnze
    PLUGIN_CODE RGan
    FORMATS ${RGAIN_FORMATS}
    PRODUCT_NAME "R-Gain"
)

target_sources(RGain
    PRIVATE
        src/PluginProcessor.cpp
        src/PluginEditor.cpp
        src/Parameters.cpp
        src/dsp/GainProcessor.cpp
)

target_include_directories(RGain
    PRIVATE
        src
)

target_compile_definitions(RGain
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(RGain
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
        ranze::plugin_ui
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
