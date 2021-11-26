/*
  ==============================================================================

    DelayClass.cpp
    Created: 9 Nov 2021 7:33:50am
    Author:  Max

  ==============================================================================
*/

#include "DelayClasses.h"

# pragma mark - MonoDelay
MonoDelay::MonoDelay()
{
    sample_rate = 44100;
    // initialise circular buffer to zeros
    for(int i=0; i<max_delay_samples; i++){
        circular_buffer[i] = 0.;
    }
}
MonoDelay::MonoDelay(int sr)
{
    sample_rate = sr;
    // initialise circular buffer to zeros
    for(int i=0; i<max_delay_samples; i++){
        circular_buffer[i] = 0.;
    }
}
MonoDelay::MonoDelay(const MonoDelay &d){
    max_delay_samples = d.get_max_delay_samples();
    for(int i=0; i<max_delay_samples; i++){
        circular_buffer[i] = 0.;
    }
    read_pointer = d.get_read_pointer();
    write_pointer = d.get_write_pointer();
    delay_time = d.get_delay_time();
    delay_samples_target = d.get_delay_samples_target();
    delay_samples_current = d.get_delay_samples_current();
    delay_samples_current_int = d.get_delay_samples_current_int();
    sample_rate = d.get_sample_rate();
    feedback = d.get_feedback();
    dry = d.get_dry();
    wet = d.get_wet();
}
MonoDelay::~MonoDelay()
{
}

int MonoDelay::get_max_delay_samples() const{
    return max_delay_samples;
}
int MonoDelay::get_read_pointer() const{
    return read_pointer;
}
int MonoDelay::get_write_pointer() const{
    return write_pointer;
}
float MonoDelay::get_delay_time() const{
    return delay_time;
}
int MonoDelay::get_delay_samples_target() const{
    return delay_samples_target;
}
float MonoDelay::get_delay_samples_current() const{
    return delay_samples_current;
}
int MonoDelay::get_delay_samples_current_int() const{
    return delay_samples_current_int;
}
int MonoDelay::get_sample_rate() const{
    return sample_rate;
}
float MonoDelay::get_feedback() const{
    return feedback;
}
float MonoDelay::get_dry() const{
    return dry;
}
float MonoDelay::get_wet() const{
    return wet;
}

MonoDelay& MonoDelay::operator=(const MonoDelay* d){
    max_delay_samples = d->get_max_delay_samples();
    for(int i=0; i<max_delay_samples; i++){
        circular_buffer[i] = 0.;
    }
    read_pointer = d->get_read_pointer();
    write_pointer = d->get_write_pointer();
    delay_time = d->get_delay_time();
    delay_samples_target = d->get_delay_samples_target();
    delay_samples_current = d->get_delay_samples_current();
    delay_samples_current_int = d->get_delay_samples_current_int();
    sample_rate = d->get_sample_rate();
    feedback = d->get_feedback();
    dry = d->get_dry();
    wet = d->get_wet();
    return *this;
}

float MonoDelay::process_sample(float s){
    // adjust delay time
    if(delay_samples_current < (float)delay_samples_target-1){
        delay_samples_current = delay_samples_current + 0.5;
    }else if(delay_samples_current > (float)delay_samples_target+1){
        delay_samples_current = delay_samples_current - 0.5;
    }
    delay_samples_current_int = (int)delay_samples_current;
    // write to circular buffer
    read_pointer = write_pointer - delay_samples_current_int < 0 ? max_delay_samples + write_pointer - delay_samples_current_int : write_pointer - delay_samples_current_int ;
    circular_buffer[ write_pointer ] = s + feedback*circular_buffer[ read_pointer ];
    write_pointer++;
    write_pointer = write_pointer%max_delay_samples;
    return dry*s + wet*circular_buffer[ read_pointer ];
}

void MonoDelay::setSampleRate(int sr){
    sample_rate = sr;
}
void MonoDelay::setDelayTime(float d){
    delay_time = d;
    delay_samples_target = (int)(delay_time*(float)sample_rate);
}
void MonoDelay::setFeedback(float f){
    feedback = f;
}
void MonoDelay::setWetDry(float w, float d){
    wet = w;
    dry = d;
}

# pragma mark - MonoLPFDelay

MonoLPFDelay::MonoLPFDelay()
{
    sample_rate = 44100;
    // initialise circular buffer to zeros
    for(int i=0; i<max_delay_samples; i++){
        circular_buffer[i] = 0.;
    }
    lpf = new LowPassFilter(sample_rate, frequency, q);
}
MonoLPFDelay::MonoLPFDelay(int sr)
{
    sample_rate = sr;
    // initialise circular buffer to zeros
    for(int i=0; i<max_delay_samples; i++){
        circular_buffer[i] = 0.;
    }
    lpf = new LowPassFilter(sample_rate, frequency, q);
}
MonoLPFDelay::~MonoLPFDelay()
{
    lpf -> ~LowPassFilter();
}

float MonoLPFDelay::process_sample(float s){
    // adjust delay time
    if(delay_samples_current < (float)delay_samples_target-1){
        delay_samples_current = delay_samples_current + 0.5;
    }else if(delay_samples_current > (float)delay_samples_target+1){
        delay_samples_current = delay_samples_current - 0.5;
    }
    delay_samples_current_int = (int)delay_samples_current;
    // write to circular buffer
    read_pointer = write_pointer - delay_samples_current_int < 0 ? max_delay_samples + write_pointer - delay_samples_current_int : write_pointer - delay_samples_current_int ;
    circular_buffer[ write_pointer ] = lpf->process_sample( s + feedback*circular_buffer[ read_pointer ] );
    write_pointer++;
    write_pointer = write_pointer%max_delay_samples;
    return dry*s + wet*circular_buffer[ read_pointer ];
}

void MonoLPFDelay::setSampleRate(int sr){
    sample_rate = sr;
}
void MonoLPFDelay::setDelayTime(float d){
    delay_time = d;
    delay_samples_target = (int)(delay_time*(float)sample_rate);
}
void MonoLPFDelay::setFeedback(float f){
    feedback = f;
}
void MonoLPFDelay::setWetDry(float w, float d){
    wet = w;
    dry = d;
}
void MonoLPFDelay::setLPFfrequency(float f){
    frequency = f;
    lpf->set_frequency(frequency);
}
void MonoLPFDelay::setLPFq(float q_in){
    q = q_in;
    lpf->set_q(q);
}
