/*
  ==============================================================================

    DistortionClasses.h
    Created: 9 Nov 2021 11:36:19pm
    Author:  Max

  ==============================================================================
*/

#pragma once


class ClippingDistortion
{
public:
    ClippingDistortion();
    ClippingDistortion(float d);
    ~ClippingDistortion();
    
    void setDistortionRate(float d);
    
    float process_sample(float s);
private:
    float dist_rate = 0.5;
    float clipping_limit = 0.5;
    float amp;
};

/*
TODO: make TanhDistortion class
 */
