/*
  ==============================================================================

    ReverbClasses.cpp
    Created: 13 Nov 2021 7:43:29am
    Author:  Max

  ==============================================================================
*/

#include "ReverbClasses.h"

D4Reverb::D4Reverb(){
    d1 = new MonoLPFDelay((int) sample_rate);
    d2 = new MonoLPFDelay((int) sample_rate);
    d3 = new MonoLPFDelay((int) sample_rate);
    d4 = new MonoLPFDelay((int) sample_rate);
    
    configure_state();
}
D4Reverb::D4Reverb(int sr){
    sample_rate = (float)sr;
    d1 = new MonoLPFDelay((int) sample_rate);
    d2 = new MonoLPFDelay((int) sample_rate);
    d3 = new MonoLPFDelay((int) sample_rate);
    d4 = new MonoLPFDelay((int) sample_rate);
    
    configure_state();
}
D4Reverb::~D4Reverb(){
    
}

void D4Reverb::set_room_size(float r){
    
}
void D4Reverb::set_damping(float d){
    
}

float D4Reverb::process_sample(float s){
    float y1 = d1->process_sample(s);
    float y2 = d2->process_sample(y1);
    float y3 = d3->process_sample(y2);
    float y4 = d4->process_sample(y3);
    return y4;
}

void D4Reverb::configure_state(){
    d1->setDelayTime(0.005);
    d1->setFeedback(0.5);
    d1->setWetDry(0.5, 1);
    
    d2->setDelayTime(0.011);
    d2->setFeedback(0.5);
    d2->setWetDry(0.5, 1);
    
    d3->setDelayTime(0.017);
    d3->setFeedback(0.5);
    d3->setWetDry(0.5, 1);
    
    d4->setDelayTime(0.023);
    d4->setFeedback(0.5);
    d4->setWetDry(0.5, 1);
}
