/*
  ==============================================================================

    FilterClasses.cpp
    Created: 12 Nov 2021 10:11:21pm
    Author:  Max

  ==============================================================================
*/

#include "FilterClasses.h"

LowPassFilter::LowPassFilter(){
    x1 = 0.;
    x2 = 0.;
    y1 = 0.;
    y2 = 0.;
    a1 = 0.;
    a2 = 0.;
    b0 = 1.;
    b1 = 0.;
    b2 = 0.;
    calculate_coefficients();
}
LowPassFilter::LowPassFilter(int sr, float f, float q_in){
    x1 = 0.;
    x2 = 0.;
    y1 = 0.;
    y2 = 0.;
    a1 = 0.;
    a2 = 0.;
    b0 = 1.;
    b1 = 0.;
    b2 = 0.;
    frequency = f;
    q = q_in;
    sample_rate = (float) sr;
    calculate_coefficients();
}
LowPassFilter::~LowPassFilter(){
    
}
void LowPassFilter::calculate_coefficients()
{
    float k = tanf(M_PI * frequency / sample_rate);
    float norm = 1.0 / (1 + k / q + k * k);
    
    b0 = k * k * norm;
    b1 = 2.0 * b0;
    b2 = b0;
    a1 = 2 * (k * k - 1) * norm;
    a2 = (1 - k / q + k * k) * norm;
}

float LowPassFilter::process_sample(float s){
    float out = b0*s + b1*x1 + b2*x2 - a1*y1 - a2*y2;
    
    x2 = x1;
    x1 = s;
    y2 = y1;
    y1 = out;
    return out;
}

void LowPassFilter::set_frequency(float f){
    frequency = f;
    calculate_coefficients();
}
void LowPassFilter::set_q(float q_in){
    q = q_in;
    calculate_coefficients();
}
void LowPassFilter::set_sample_rate(int sr){
    sample_rate = (float) sr;
    calculate_coefficients();
}
