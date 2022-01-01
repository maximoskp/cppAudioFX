/*
  ==============================================================================

    PitchShiftClasses.h
    Created: 31 Dec 2021 4:19:58pm
    Author:  Max

  ==============================================================================
*/

#pragma once

// #include <JuceHeader.h>
#include <iostream>

using namespace std;

class OctaveDown{
public:
    OctaveDown();
    ~OctaveDown();
    
    float process_sample(float s);
    
private:
    int p;
    float v[1024];
    float x[2048];
};
