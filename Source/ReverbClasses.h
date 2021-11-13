/*
  ==============================================================================

    ReverbClasses.h
    Created: 13 Nov 2021 7:43:29am
    Author:  Max

  ==============================================================================
*/

// https://github.com/sellicott/DSP-FFMpeg-Reverb

#pragma once
#include "DelayClasses.h"

class D4Reverb
{
public:
    D4Reverb();
    D4Reverb(int sr);
    ~D4Reverb();
    
    void set_room_size(float r);
    void set_damping(float d);
    
    float process_sample(float s);

private:
    float sample_rate = 44100.;
    float room_size = 1.;
    float damping = 1.;
    MonoLPFDelay *d1, *d2, *d3, *d4;
    void configure_state();
};
