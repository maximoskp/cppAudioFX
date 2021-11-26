/*
  ==============================================================================

    ReverbClasses.h
    Created: 13 Nov 2021 7:43:29am
    Author:  Max

  ==============================================================================
*/

// https://github.com/sellicott/DSP-FFMpeg-Reverb

// https://signalsmith-audio.co.uk/writing/2021/lets-write-a-reverb/

#pragma once
#include "DelayClasses.h"
#include "FilterClasses.h"
#include <iostream>
// #include <vector>

using namespace std;

class Diffuser
{
public:
    Diffuser();
    Diffuser(int sr);
    // Diffuser(int num_channels); // complete when dynamic
    ~Diffuser();
    
    void set_delay_time_factor(float m, float v);
    void set_delay_wet_factor(float m, float v);
    void set_feedback_factor(float m, float v);
    float process_sample(float s);
    
    bool get_all_initialised();
    
private:
    // TODO: make all the following dynamic
    int num_channels = 8;
    // vector<MonoDelay *> delays;
    // MonoDelay *delay;
    MonoDelay delays[8];
    float hadamard[8][8];
    // delay time factor
    float deltime_mean = 0.02;
    float deltime_var = 0.01;
    // delay wet factor
    float delwet_mean = 0.5;
    float delwet_var = 0.01;
    // feedback factor
    float delfeedback_mean = 0.5;
    float delfeedback_var = 0.01;
    
    bool all_initialised = false;
    
    void makeHadamard();
    void initialize_delays();
    void initialize_delays(int sr);
};
