/*
  ==============================================================================

    FilterClasses.h
    Created: 12 Nov 2021 10:11:21pm
    Author:  Max

  ==============================================================================
*/

#pragma once
#include <cmath>

class LowPassFilter
{
public:
    LowPassFilter();
    LowPassFilter(int sample_rate, float f, float q_in);
    ~LowPassFilter();
    
    float process_sample(float s);
    
    void set_frequency(float f);
    void set_q(float q_in);
    void set_sample_rate(int sr);
private:
    float frequency = 500;
    float q = 1;
    float sample_rate;
    
    // filter states
    float x1, x2, y1, y2;
    // filter coefficients
    float b0, b1, b2, a1, a2;
    
    void calculate_coefficients();
};
