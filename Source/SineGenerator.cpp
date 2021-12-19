/*
  ==============================================================================

    SineGenerator.cpp
    Created: 9 Dec 2020 5:10:56pm
    Author:  user

  ==============================================================================
*/

#include "SineGenerator.h"

// constructor and destructor
SineGenerator::SineGenerator() {
    // default constructor
    sampleRate = 44100.;
    frequency = 440.;
    amplitude = 0.7;
    current_angle = 0.;
}
SineGenerator::SineGenerator(float sr) {
    // default constructor
    sampleRate = sr;
    frequency = 440.;
    amplitude = 0.7;
    current_angle = 0.;
}
SineGenerator::SineGenerator(float sr, float freq, float amp) {
    // default constructor
    sampleRate = sr;
    frequency = freq;
    amplitude = amp;
    current_angle = 0.;
}
SineGenerator::~SineGenerator() {
    // do nothing...
}

// public functions for setting private variables
void SineGenerator::setFrequency(float f) {
    frequency = f;
    std::cout << "frequency: " << frequency << std::endl;
}
void SineGenerator::setAmplitude(float a) {
    amplitude = a;
}
float SineGenerator::getNextSample() {
    current_sample_value = amplitude * std::sin(current_angle);
    current_angle += 2.0 * pi * frequency / sampleRate;
    // since we don't have infinite precision in pi, we need to keep 
    // the angle value within one turn of the unit circle
    while (current_angle > 2. * pi) {
        current_angle -= 2. * pi;
    }
    return current_sample_value;
}
void SineGenerator::setSampleRate(float s) {
    sampleRate = s;
}
