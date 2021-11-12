#pragma once

#include <JuceHeader.h>
#include "DelayClasses.h"
#include "DistortionClasses.h"
#include "FilterClasses.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    MonoDelay *delay;
    ClippingDistortion *dist;
    LowPassFilter *lpf;

private:
    //==============================================================================
    // Your private member variables go here...
//    int max_delay_samples = 5*44100;
//    float circular_buffer[5*44100];
//    int read_pointer = 0;
//    int write_pointer = 0;
    float delay_time = .25;
//    int delay_samples_target = 2*44100;
//    int delay_samples_current = 2*44100;
    int sample_rate = 0;
    float feedback = 0.5;
    float dry = 0.0;
    float wet = 0.5;
    
    float dist_rate = 0.5;
    
    float lpf_freq = 500;
    float lpf_q = 5.;
    
    // UI
    juce::Slider dry_slider;
    juce::Slider wet_slider;
    juce::Slider time_slider;
    juce::Slider feedback_slider;
    
    juce::Slider dist_slider;
    
    juce::Slider lpf_freq_slider;
    juce::Slider lpf_q_slider;
    
    // functions
    void changeDry();
    void changeWet();
    void changeTime();
    void changeFeedback();
    
    void changeDistRate();
    
    void changeLPFfreq();
    void changeLPFq();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
