/*
  ==============================================================================

    SineGenerator.h
    Created: 9 Dec 2020 5:10:56pm
    Author:  user

  ==============================================================================
*/

#pragma once
#include <iostream>
#include <cmath>

using namespace std;

class SineGenerator {
public:
    // constructor and destructor
    SineGenerator();
    SineGenerator(float sr);
    SineGenerator(float sr, float freq, float amp);
    ~SineGenerator();

    void setFrequency(float f);
    void setAmplitude(float a);
    float getNextSample();
    void setSampleRate(float s);
private:
    float sampleRate;
    float frequency;
    float amplitude;

    float current_angle;
    float current_sample_value;

    float pi = 3.14159265358979323846;
};
