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
#include <vector>
#include <random>


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
    float deltime_mean = 0.002;
    float deltime_var = 0.001;
    // delay wet factor
    float delwet_mean = 0.5;
    float delwet_var = 0.01;
    // feedback factor
    float delfeedback_mean = 0.5;
    float delfeedback_var = 0.01;
    
    float diffusor_dry = 0.5;
    
    bool all_initialised = false;
    
    void makeHadamard();
    void initialize_constants();
    void initialize_delays();
    void initialize_delays(int sr);
};

class Reverb8Diff
{
public:
    Reverb8Diff();
    Reverb8Diff(int sr);
    ~Reverb8Diff();
    
    float process_sample(float s);
    
    void set_room_size(float x);
    void set_lpf(float x);
    void set_wet(float x);
    void set_dry(float x);
    
private:
    int sample_rate = 44100;
    int num_of_diffusers = 5;
    vector<Diffuser*> diffusers;
    Diffuser *diffuser;
    
    int num_of_lops = 4;
    vector<LowPassFilter*> lops;
    LowPassFilter *lop;
    
    double delay_mean_min = 0.007;
    double delay_mean_max = 0.065;
    double delay_var_min = 0.005;
    double delay_var_max = 0.035;
    double feedback_mean_min = 0.3;
    double feedback_mean_max = 0.9;
    double feedback_var = 0.15;
    
    double delay_mean_step;
    double delay_var_step;
    double feedback_mean_step;

    random_device rd; // obtain a random number from hardware
    
    float wet;
    float dry;
};
