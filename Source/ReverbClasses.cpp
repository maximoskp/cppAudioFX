/*
  ==============================================================================

    ReverbClasses.cpp
    Created: 13 Nov 2021 7:43:29am
    Author:  Max

  ==============================================================================
*/

#include "ReverbClasses.h"

#pragma mark - diffuser

Diffuser::Diffuser(){
    initialize_constants();
    makeHadamard();
    initialize_delays();
    std::cout << "all done\n";
}
Diffuser::Diffuser(int sr){
    initialize_constants();
    makeHadamard();
    initialize_delays(sr);
    std::cout << "all done\n";
}
Diffuser::~Diffuser(){
    for(int i=0; i<num_channels; i++){
        // delete delays[i];
        delays[i] = NULL;
    }
}

void Diffuser::set_delay_time_factor(float m, float v){
    deltime_mean = m;
    deltime_var = v;
    for(int i=0; i<8; i++){
        float r = deltime_mean - deltime_var + 2.*deltime_var*(float)(rand()%1000)/1000.;
        delays[i].setDelayTime(r);
    }
}
void Diffuser::set_delay_wet_factor(float m, float v){
    delwet_mean = m;
    delwet_var = v;
    for(int i=0; i<8; i++){
        float r = delwet_mean - delwet_var + 2.*delwet_var*(float)(rand()%1000)/1000.;
        delays[i].setWetDry(r, 0.);
    }
}
void Diffuser::set_feedback_factor(float m, float v){
    delfeedback_mean = m;
    delfeedback_var = v;
    for(int i=0; i<8; i++){
        float r = delfeedback_mean - delfeedback_var + 2.*delfeedback_var*(float)(rand()%1000)/1000.;
        delays[i].setFeedback(r);
    }
}

bool Diffuser::get_all_initialised(){
    return all_initialised;
}

void Diffuser::makeHadamard(){
    std::cout << "makeHadamard\n";
    hadamard[0][0] = 1/sqrtf((float)num_channels);
    for(int x=1; x<num_channels; x+=x){
        for(int i=0; i<x; i++){
            for(int j=0; j<x; j++){
                hadamard[i+x][j] = hadamard[i][j];
                hadamard[i][j+x] = hadamard[i][j];
                hadamard[i+x][j+x] = -hadamard[i][j];
            }
        }
    }
    // random shuffling and polarity inversion of rows and columns
    // rows
    float tmp_v;
    for(int i=0; i<num_channels; i++){
        // pick a random row
        int r = rand()%num_channels;
        // copy start to temp
        for(int j=0; j<num_channels; j++){
            tmp_v = hadamard[i][j];
            hadamard[i][j] = hadamard[r][j];
            hadamard[r][j] = tmp_v;
        }
    }
    // columns
    for(int j=0; j<num_channels; j++){
        // pick a random row
        int r = rand()%num_channels;
        // copy start to temp
        for(int i=0; i<num_channels; i++){
            tmp_v = hadamard[i][j];
            hadamard[i][j] = hadamard[i][r];
            hadamard[i][r] = tmp_v;
        }
    }
    // print hadamard
    for(int i=0; i<num_channels; i++){
        for(int j=0; j<num_channels; j++){
            std::cout << hadamard[i][j] << "\t";
        }
        std::cout << "\n";
    }
}

void Diffuser::initialize_constants(){
    std::cout << "initialize_constants\n";
    deltime_mean = 0.008;
    deltime_var = 0.007;
    delwet_mean = 1.;
    delwet_var = 0.01;
    delfeedback_mean = 0.5;
    delfeedback_var = 0.01;
    diffusor_dry = 0.5;
}

void Diffuser::initialize_delays(){
    std::cout << "initialize_delays 0\n";
    deltime_mean = 0.008;
    deltime_var = 0.007;
    delwet_mean = 1.;
    delwet_var = 0.01;
    delfeedback_mean = 0.5;
    delfeedback_var = 0.01;
    diffusor_dry = 0.5;
    for(int i=0; i<8; i++){
        delays[i] = MonoDelay();
        float r = deltime_mean - deltime_var + 2.*deltime_var*(float)(rand()%1000)/1000.;
        std::cout << "time 0: " << deltime_mean << " - " << r << endl;
        delays[i].setDelayTime(r);
        r = delwet_mean - delwet_var + 2.*delwet_var*(float)(rand()%1000)/1000.;
        delays[i].setWetDry(r, 0.);
        r = delfeedback_mean - delfeedback_var + 2.*delfeedback_var*(float)(rand()%1000)/1000.;
        delays[i].setFeedback(r);
    }
    all_initialised = true;
}
void Diffuser::initialize_delays(int sr){
    std::cout << "initialize_delays 1\n";
    deltime_mean = 0.150;
    deltime_var = 0.140;
    delwet_mean = 1.;
    delwet_var = 0.01;
    delfeedback_mean = 0.6;
    delfeedback_var = 0.01;
    diffusor_dry = 0.5;
    for(int i=0; i<8; i++){
        delays[i] = MonoDelay(sr);
        float r = deltime_mean - deltime_var + 2.*deltime_var*(float)(rand()%1000)/1000.;
        std::cout << "time 1: " << deltime_mean << " - " << r << endl;
        delays[i].setDelayTime(r);
        r = delwet_mean - delwet_var + 2.*delwet_var*(float)(rand()%1000)/1000.;
        delays[i].setWetDry(r, 0.);
        r = delfeedback_mean - delfeedback_var + 2.*delfeedback_var*(float)(rand()%1000)/1000.;
        delays[i].setFeedback(r);
    }
    all_initialised = true;
}

float Diffuser::process_sample(float s){
    float sample_out = 0.;
    for(int i=0; i<num_channels; i++){
        // all wet
        // sample_out = delays[i].process_sample(s);
        // hadamard
        float channel_sample = 0.;
        for(int j=0; j<num_channels; j++){
            channel_sample = hadamard[i][j]*delays[i].process_sample(s);
        }
        sample_out += channel_sample;
    }
    // add dry
    // sample_out += diffusor_dry*s;
    return sample_out;
}

# pragma mark - Reverb8Diff

Reverb8Diff::Reverb8Diff(){
    sample_rate = 44100;
    diffuser1 = new Diffuser(sample_rate);
    diffuser1->set_delay_time_factor(0.015, 0.005);
    diffuser1->set_feedback_factor(0.8, 0.05);
    
    diffuser2 = new Diffuser(sample_rate);
    diffuser2->set_delay_time_factor(0.025, 0.015);
    diffuser2->set_feedback_factor(0.8, 0.05);
    
    diffuser3 = new Diffuser(sample_rate);
    diffuser3->set_delay_time_factor(0.065, 0.015);
    diffuser3->set_feedback_factor(0.8, 0.05);
    
    diffuser4 = new Diffuser(sample_rate);
    diffuser4->set_delay_time_factor(0.105, 0.025);
    diffuser4->set_feedback_factor(0.8, 0.05);
    
    diffuser5 = new Diffuser(sample_rate);
    diffuser5->set_delay_time_factor(0.165, 0.035);
    diffuser5->set_feedback_factor(0.8, 0.05);
    
    lop1 = new LowPassFilter(sample_rate, 500., 0.5);
    
    wet = 1.;
}
Reverb8Diff::Reverb8Diff(int sr){
    sample_rate = sr;
    diffuser1 = new Diffuser(sample_rate);
    diffuser1->set_delay_time_factor(0.007, 0.005);
    diffuser1->set_feedback_factor(0.3, 0.15);
    
    diffuser2 = new Diffuser(sample_rate);
    diffuser2->set_delay_time_factor(0.017, 0.015);
    diffuser2->set_feedback_factor(0.4, 0.15);
    
    diffuser3 = new Diffuser(sample_rate);
    diffuser3->set_delay_time_factor(0.035, 0.025);
    diffuser3->set_feedback_factor(0.5, 0.15);
    
    diffuser4 = new Diffuser(sample_rate);
    diffuser4->set_delay_time_factor(0.065, 0.035);
    diffuser4->set_feedback_factor(0.6, 0.15);
    
    diffuser5 = new Diffuser(sample_rate);
    diffuser5->set_delay_time_factor(0.565, 0.135);
    diffuser5->set_feedback_factor(0.85, 0.05);
    
    lop1 = new LowPassFilter(sample_rate, 700., 0.5);
    lop2 = new LowPassFilter(sample_rate, 2800., 0.5);
    lop3 = new LowPassFilter(sample_rate, 2500., 0.5);
    lop4 = new LowPassFilter(sample_rate, 1000., 0.5);
    
    wet = 1.;
}
Reverb8Diff::~Reverb8Diff(){
    
}

float Reverb8Diff::process_sample(float s){
    float s_diffuse = s;
    s_diffuse = diffuser1->process_sample(s_diffuse);
    s_diffuse = lop1->process_sample(s_diffuse);
    
    s_diffuse = diffuser2->process_sample(s_diffuse);
    s_diffuse = lop2->process_sample(s_diffuse);
    
    s_diffuse = diffuser3->process_sample(s_diffuse);
    s_diffuse = lop3->process_sample(s_diffuse);
    
    s_diffuse = diffuser4->process_sample(s_diffuse);
    s_diffuse = lop4->process_sample(s_diffuse);
    
    s_diffuse = diffuser5->process_sample(s_diffuse);
    return dry*s + wet*s_diffuse;
}

void Reverb8Diff::set_room_size(float x){
    diffuser1->set_delay_time_factor( (1.+x)*0.007, 0.005);
    diffuser1->set_feedback_factor( (0.6+x/2.)*0.3, 0.15);

    diffuser2->set_delay_time_factor( (1.+x)*0.017, 0.015);
    diffuser2->set_feedback_factor( (0.6+x/2.)*0.4, 0.15);

    diffuser3->set_delay_time_factor( (1.+x)*0.035, 0.025);
    diffuser3->set_feedback_factor( (0.6+x/2.)*0.5, 0.15);

    diffuser4->set_delay_time_factor( (1.+x)*0.065, 0.035);
    diffuser4->set_feedback_factor( (0.6+x/2.)*0.6, 0.15);
    
    diffuser5->set_delay_time_factor( (0.3+x)*0.565 , 0.135 );
    diffuser5->set_feedback_factor( (0.5+x/2.)*0.9, 0.05);
}
void Reverb8Diff::set_lpf(float x){
    
}
void Reverb8Diff::set_wet(float x){
    wet = x;
}
void Reverb8Diff::set_dry(float x){
    dry = x;
}
