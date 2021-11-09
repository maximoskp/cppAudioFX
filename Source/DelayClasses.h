/*
  ==============================================================================

    DelayClass.h
    Created: 9 Nov 2021 7:33:50am
    Author:  Max

  ==============================================================================
*/

#pragma once

class MonoDelay
{
public:
    MonoDelay();
    MonoDelay(int sr);
    ~MonoDelay();
    // setters
    void setSampleRate(int sr);
    void setDelayTime(float d);
    void setFeedback(float f);
    void setWetDry(float w, float d);
    // process audio
    float process_sample(float s);
private:
    int max_delay_samples = 5*44100;
    float circular_buffer[5*44100];
    int read_pointer = 0;
    int write_pointer = 0;
    float delay_time = .25;
    int delay_samples_target = 2*44100;
    float delay_samples_current = 2.*44100.;
    int delay_samples_current_int = 2*44100;
    int sample_rate = 0;
    float feedback = 0.5;
    float dry = 0.0;
    float wet = 0.5;
};
