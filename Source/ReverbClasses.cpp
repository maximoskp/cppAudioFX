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
    makeHadamard();
    initialize_delays();
    std::cout << "all done\n";
}
Diffuser::Diffuser(int sr){
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
}
void Diffuser::set_delay_wet_factor(float m, float v){
    delwet_mean = m;
    delwet_var = v;
}
void Diffuser::set_feedback_factor(float m, float v){
    delfeedback_mean = m;
    delfeedback_var = v;
}

bool Diffuser::get_all_initialised(){
    return all_initialised;
}

void Diffuser::makeHadamard(){
    std::cout << "makeHadamard 0\n";
    hadamard[0][0] = 1/sqrtf((float)num_channels);
    for(int x=1; x<num_channels; x+=x){
        for(int i=0; i<num_channels; i++){
            for(int j=0; j<num_channels; j++){
                hadamard[i+x][j] = hadamard[i][j];
                hadamard[i][j+x] = hadamard[i][j];
                hadamard[i+x][j+x] = -hadamard[i][j];
            }
        }
    }
    // random shuffling and polarity inversion of rows and columns
    // rows
    std::cout << "makeHadamard 1\n";
    int tmp_v;
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
    std::cout << "makeHadamard 2\n";
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
}

void Diffuser::initialize_delays(){
    std::cout << "initialize_delays\n";
//    for(int i=0; i<num_channels; i++){
//        // delays[i] = new MonoDelay();
//        delays.push_back(MonoDelay());
//        std::cout << "initialize_delays - " << i << "\n";
//        float r = deltime_mean - deltime_var + 2.*deltime_var*(float)(rand()%1000)/1000.;
//        delays[i].setDelayTime(r);
//        r = delwet_mean - delwet_var + 2.*delwet_var*(float)(rand()%1000)/1000.;
//        delays[i].setWetDry(r, 1.);
//        r = delfeedback_mean - delfeedback_var + 2.*delfeedback_var*(float)(rand()%1000)/1000.;
//        delays[i].setFeedback(r);
//    }
    for(int i=0; i<8; i++){
        // MonoDelay *delay = new MonoDelay();
        delays[i] = MonoDelay();
        float r = deltime_mean - deltime_var + 2.*deltime_var*(float)(rand()%1000)/1000.;
        // delay->setDelayTime(r);
        delays[i].setDelayTime(r);
        r = delwet_mean - delwet_var + 2.*delwet_var*(float)(rand()%1000)/1000.;
        // delay->setWetDry(r, 1.);
        delays[i].setWetDry(r, 1.);
        r = delfeedback_mean - delfeedback_var + 2.*delfeedback_var*(float)(rand()%1000)/1000.;
        // delay->setFeedback(r);
        delays[i].setFeedback(r);
        // delays[i] = delay;
        // delays.push_back(delay);
        // delays.emplace_back(delay);
    }
    all_initialised = true;
}
void Diffuser::initialize_delays(int sr){
    std::cout << "initialize_delays\n";
//    for(int i=0; i<num_channels; i++){
//        // delays[i] = new MonoDelay(sr);
//        delays.push_back(MonoDelay(sr));
//        std::cout << "initialize_delays - " << i << "\n";
//        float r = deltime_mean - deltime_var + 2.*deltime_var*(float)(rand()%1000)/1000.;
//        delays[i].setDelayTime(r);
//        r = delwet_mean - delwet_var + 2.*delwet_var*(float)(rand()%1000)/1000.;
//        delays[i].setWetDry(r, 1.);
//        r = delfeedback_mean - delfeedback_var + 2.*delfeedback_var*(float)(rand()%1000)/1000.;
//        delays[i].setFeedback(r);
//    }
    // delays = new MonoDelay[8];
    for(int i=0; i<8; i++){
        // MonoDelay *delay = new MonoDelay(sr);
        delays[i] = MonoDelay(sr);
        float r = deltime_mean - deltime_var + 2.*deltime_var*(float)(rand()%1000)/1000.;
        // delay->setDelayTime(r);
        delays[i].setDelayTime(r);
        r = delwet_mean - delwet_var + 2.*delwet_var*(float)(rand()%1000)/1000.;
        // delay->setWetDry(r, 1.);
        delays[i].setWetDry(r, 1.);
        r = delfeedback_mean - delfeedback_var + 2.*delfeedback_var*(float)(rand()%1000)/1000.;
        // delay->setFeedback(r);
        delays[i].setFeedback(r);
        // delays[i] = delay;
        // delays.push_back(delay);
        // delays.emplace_back(delay);
    }
    all_initialised = true;
}

float Diffuser::process_sample(float s){
    // std::cout << "process_sample\n";
//    float sample_out = 0.;
//    for(int i=0; i<num_channels; i++){
//        // std::cout << "process_sample of " << i << "\n";
//        // sample_out += delays[i].process_sample( s );
//    }
    float sample_out = 0.;
    for(int i=0; i<num_channels; i++){
        // sample_out = delays[i]->process_sample(s);
        sample_out = delays[i].process_sample(s);
    }
    return sample_out;
}
