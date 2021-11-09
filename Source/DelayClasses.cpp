/*
  ==============================================================================

    DelayClass.cpp
    Created: 9 Nov 2021 7:33:50am
    Author:  Max

  ==============================================================================
*/

#include "DelayClasses.h"

MonoDelay::MonoDelay()
{
    sample_rate = 44100;
    // initialise circular buffer to zeros
    for(int i=0; i<max_delay_samples; i++){
        circular_buffer[i] = 0.;
    }
}
MonoDelay::MonoDelay(int sr)
{
    sample_rate = sr;
    // initialise circular buffer to zeros
    for(int i=0; i<max_delay_samples; i++){
        circular_buffer[i] = 0.;
    }
}
MonoDelay::~MonoDelay()
{
}

float MonoDelay::process_sample(float s){
    // adjust delay time
    if(delay_samples_current < (float)delay_samples_target-1){
        delay_samples_current = delay_samples_current + 0.5;
    }else if(delay_samples_current > (float)delay_samples_target+1){
        delay_samples_current = delay_samples_current - 0.5;
    }
    delay_samples_current_int = (int)delay_samples_current;
    // write to circular buffer
    read_pointer = write_pointer - delay_samples_current_int < 0 ? max_delay_samples + write_pointer - delay_samples_current_int : write_pointer - delay_samples_current_int ;
    circular_buffer[ write_pointer ] = s + feedback*circular_buffer[ read_pointer ];
    write_pointer++;
    write_pointer = write_pointer%max_delay_samples;
    return dry*s + wet*circular_buffer[ read_pointer ];
}

void MonoDelay::setSampleRate(int sr){
    sample_rate = sr;
}
void MonoDelay::setDelayTime(float d){
    delay_time = d;
    delay_samples_target = (int)(delay_time*(float)sample_rate);
}
void MonoDelay::setFeedback(float f){
    feedback = f;
}
void MonoDelay::setWetDry(float w, float d){
    wet = w;
    dry = d;
}
