#pragma once

#include <JuceHeader.h>
#include "DelayClasses.h"
#include "DistortionClasses.h"
#include "ReverbClasses.h"
#include "SineGenerator.h"
#include "Chorus.h"
//#include "PitchShiftClasses.h"

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
    
    MonoLPFDelay *delay;
    ClippingDistortion *dist;
    Reverb8Diff *reverb;
    Chorus *chorus;
//    OctaveDown *octDown;

private:
    //==============================================================================
    // Your private member variables go here...
    // UI configuration
    float x_offset = 30.;
    float x_size = 100.;
    float y_offset = 10;
    float y_step = 40.;
    
    float delay_time = .25;
    int sample_rate = 0;
    float feedback = 0.5;
    float dry = 0.0;
    float wet = 0.5;
    
    float dist_rate = 0.5;
    
    float lpf_freq = 500;
    float lpf_q = 5.;
    
    float depth = 50.;
    float speed = 10.;
    
    // UI
    juce::Label delay_label;
    juce::Slider dry_slider;
    juce::Slider wet_slider;
    juce::Slider time_slider;
    juce::Slider feedback_slider;
    juce::Slider lpf_freq_slider;
    juce::Slider lpf_q_slider;
    
    juce::Label dist_label;
    juce::Slider dist_slider;
    
    juce::Label chorus_label;
    juce::Slider depth_slider;
    juce::Slider speed_slider;
    
    juce::Label reverb_label;
    juce::Slider room_slider;
    juce::Slider reverb_lpf_slider;
    juce::Slider reverb_wet_slider;
    juce::Slider reverb_dry_slider;
    
    // functions
    void changeDry();
    void changeWet();
    void changeTime();
    void changeFeedback();
    void changeLPFfreq();
    void changeLPFq();
    
    void changeDistRate();
    
    void changeDepth();
    void changeSpeed();
    
    void changeRoom();
    void changeReverbLPF();
    void changeReverbWet();
    void changeReverbDry();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
