/*
  ==============================================================================

    DistortionClasses.cpp
    Created: 9 Nov 2021 11:36:19pm
    Author:  Max

  ==============================================================================
*/

#include "DistortionClasses.h"


ClippingDistortion::ClippingDistortion(){
    
}
ClippingDistortion::ClippingDistortion(float d){
    
}
ClippingDistortion::~ClippingDistortion(){
    
}
void ClippingDistortion::setDistortionRate(float d){
    dist_rate = d;
    clipping_limit = 1. - 0.6*dist_rate;
    amp = 1./clipping_limit;
}

float ClippingDistortion::process_sample(float s){
    float s_amp = s*amp;
    return s_amp > 0 && s_amp > clipping_limit ? clipping_limit : s_amp < 0 && s_amp < -clipping_limit ? -clipping_limit : s_amp ;
}
