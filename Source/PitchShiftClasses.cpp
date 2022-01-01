/*
  ==============================================================================

    PitchShiftClasses.cpp
    Created: 31 Dec 2021 4:19:58pm
    Author:  Max

  ==============================================================================
*/

#include "PitchShiftClasses.h"

OctaveDown::OctaveDown(){
    p = 0;
    int n = 1024;
    for(int i=0; i<n; i++){
        v[i] = (float)(i+.5)/(float)n;
        // std::cout << v[i];
        x[i] = 0.;
        x[i+n] = 0.;
    }
}
OctaveDown::~OctaveDown(){
    
}

float OctaveDown::process_sample(float s){
    p %= 2048;
    x[p] = s;
    float out = v[p%1024]*x[p-p%2] + v[(2047-p)%1024]*x[(p-p%2+1025)%2048];
    p++;
//    p %= 4;
//    x[p] = s;
//    float out = v[p%2]*x[p-p%2] + v[(p+1)%2]*x[(p-p%2+3)%4];
//    p++;
    // DBG("return_sample: " + juce::String(return_sample));
    return out;
}
