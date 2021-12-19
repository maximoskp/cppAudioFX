/*
  ==============================================================================

    Chorus.cpp
    Created: 18 Dec 2021 10:27:58pm
    Author:  Max

  ==============================================================================
*/

#include "Chorus.h"

Chorus::Chorus(){
    sample_rate = 44100;
    depth = 0.003;
    speed = 3.;
    depth_multiplier = 0.003;
    
    center_delay_time =  2.*depth + 0.001;
    
    delay = new MonoDelay( sample_rate );
    delay->setDelayTime( center_delay_time );
    delay->setFeedback(0.);
    delay->setWetDry(1., 0.);
    sine = new SineGenerator( sample_rate, speed, depth );
}
Chorus::Chorus(float sr){
    sample_rate = sr;
    depth = 0.003;
    speed = 3.;
    depth_multiplier = 0.003;
    
    center_delay_time =  2.*depth + 0.001;
    
    delay = new MonoDelay( sample_rate );
    delay->setDelayTime( center_delay_time );
    delay->setFeedback(0.);
    delay->setWetDry(1., 0.);
    sine = new SineGenerator( sample_rate, speed, depth );
}
Chorus::Chorus(float sr, float d, float s){
    sample_rate = sr;
    depth = d;
    speed = s;
    depth_multiplier = 0.003;
    
    center_delay_time =  2.*depth + 0.001;
    
    delay = new MonoDelay( sample_rate );
    delay->setDelayTime( center_delay_time );
    delay->setFeedback(0.);
    delay->setWetDry(1., 0.);
    sine = new SineGenerator( sample_rate, speed, depth );
}
Chorus::~Chorus(){
    
}

void Chorus::setSampleRate(int sr){
    sample_rate = sr;
}
void Chorus::setDepth(float d){
    depth = depth_multiplier*d;
    center_delay_time =  2.*depth + 0.001;
    std::cout << "depth: " << depth << std::endl;
    sine->setAmplitude(depth);
}
void Chorus::setSpeed(float s){
    speed = s;
    sine->setFrequency(s);
}
float Chorus::process_sample(float s){
    float sine_sample = sine->getNextSample();
    // DBG("delay time: " + juce::String(center_delay_time + sine_sample));
    delay->setDelayTime( center_delay_time + sine_sample );
    return s + (1 - depth + s)*delay->process_sample(s);
}
