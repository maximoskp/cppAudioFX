#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    // initialise circular buffer to zeros
//    for(int i=0; i<max_delay_samples; i++){
//        circular_buffer[i] = 0.;
//    }
    
    // initialise UI
    addAndMakeVisible(dry_slider);
    dry_slider.setRange(0., 1., 0.01);
    dry_slider.setValue(1.0);
    dry_slider.setTextValueSuffix(" dry");
    dry_slider.onValueChange = [this] { changeDry(); };
    
    addAndMakeVisible(wet_slider);
    wet_slider.setRange(0., 1., 0.01);
    wet_slider.setValue(1.0);
    wet_slider.setTextValueSuffix(" wet");
    wet_slider.onValueChange = [this] { changeWet(); };
    
    addAndMakeVisible(time_slider);
    time_slider.setRange(0., 5., 0.01);
    time_slider.setValue(0.25);
    time_slider.setTextValueSuffix(" secs");
    time_slider.onValueChange = [this] { changeTime(); };
    
    addAndMakeVisible(feedback_slider);
    feedback_slider.setRange(0., 1., 0.01);
    feedback_slider.setValue(0.5);
    feedback_slider.setTextValueSuffix(" feed");
    feedback_slider.onValueChange = [this] { changeFeedback(); };
    
    addAndMakeVisible(dist_slider);
    dist_slider.setRange(0., 1., 0.01);
    dist_slider.setValue(0.5);
    dist_slider.setTextValueSuffix(" dist");
    dist_slider.onValueChange = [this] { changeDistRate(); };
    
    addAndMakeVisible(lpf_freq_slider);
    lpf_freq_slider.setRange(0., 1000., 1.);
    lpf_freq_slider.setValue(500);
    lpf_freq_slider.setTextValueSuffix(" Hz");
    lpf_freq_slider.onValueChange = [this] { changeLPFfreq(); };
    
    addAndMakeVisible(lpf_q_slider);
    lpf_q_slider.setRange(0.5, 10., 0.1);
    lpf_q_slider.setValue(1);
    lpf_q_slider.setTextValueSuffix(" Q");
    lpf_q_slider.onValueChange = [this] { changeLPFq(); };
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    sample_rate = (float)sampleRate;
    delay = new MonoDelay(sample_rate);
    dist = new ClippingDistortion(dist_rate);
    lpf = new LowPassFilter( sampleRate, 500., 1. );
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!
    
    // For more details, see the help for AudioProcessor::getNextAudioBlock()
//    delay_samples_target = (int)floorf(delay_time*(float)sample_rate);
    auto* inReadBuffer = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);
    auto* leftWriteBuffer  = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
    auto* rightWriteBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
    for (auto i = 0; i < bufferToFill.numSamples; i++){
        float dist_sample = dist->process_sample(inReadBuffer[i]);
        float lpf_sample = lpf->process_sample(dist_sample);
        float tmp_delay = delay->process_sample(lpf_sample);
        leftWriteBuffer[i] = tmp_delay;
        rightWriteBuffer[i] = tmp_delay;
        // write to circular buffer
//        read_pointer = write_pointer - delay_samples_current < 0 ? max_delay_samples + write_pointer - delay_samples_current : write_pointer - delay_samples_current ;
//        circular_buffer[ write_pointer ] = inReadBuffer[i] + feedback*circular_buffer[ read_pointer ];
//        write_pointer++;
//        write_pointer = write_pointer%max_delay_samples;
//        leftWriteBuffer[i]  = dry*inReadBuffer[i] + wet*circular_buffer[ read_pointer ];
//        rightWriteBuffer[i] = dry*inReadBuffer[i] + wet*circular_buffer[ read_pointer ];
//        if(delay_samples_current < delay_samples_target){
//            delay_samples_current = (int)floorf(delay_samples_current + 0.1);
//        }else if(delay_samples_current > delay_samples_target){
//            delay_samples_current = (int)floorf(delay_samples_current - 0.1);
//        }
    }
//    DBG("write_pointer: "+juce::String(delay->write_pointer));
//    DBG("read_pointer: "+juce::String(delay->read_pointer));
    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
//     bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    wet_slider.setBounds(getWidth()/5. + 10, 50, 3.*getWidth()/4. - 20, 30);
    dry_slider.setBounds(getWidth()/5. + 10, 100, 3.*getWidth()/4. - 20, 30);
    time_slider.setBounds(getWidth()/5. + 10, 150, 3.*getWidth()/4. - 20, 30);
    feedback_slider.setBounds(getWidth()/5. + 10, 200, 3.*getWidth()/4. - 20, 30);
    
    dist_slider.setBounds(getWidth()/5. + 10, 300, 3.*getWidth()/4. - 20, 30);
    
    lpf_freq_slider.setBounds(getWidth()/5. + 10, 400, 3.*getWidth()/4. - 20, 30);
    lpf_q_slider.setBounds(getWidth()/5. + 10, 450, 3.*getWidth()/4. - 20, 30);
}

void MainComponent::changeDry(){
    dry = dry_slider.getValue();
    delay->setWetDry(wet, dry);
    DBG("dry");
}
void MainComponent::changeWet(){
    wet = wet_slider.getValue();
    delay->setWetDry(wet, dry);
    DBG("wet");
}
void MainComponent::changeTime(){
    delay_time = time_slider.getValue();
    delay->setDelayTime(delay_time);
    DBG("time");
}
void MainComponent::changeFeedback(){
    feedback = feedback_slider.getValue();
    delay->setFeedback(feedback);
    DBG("feedback");
}

void MainComponent::changeDistRate(){
    dist_rate = dist_slider.getValue();
    dist->setDistortionRate(dist_rate);
    DBG("distortion");
}

void MainComponent::changeLPFfreq(){
    lpf_freq = lpf_freq_slider.getValue();
    lpf->set_frequency(lpf_freq);
    DBG("lpf frequency");
}

void MainComponent::changeLPFq(){
    lpf_q = lpf_q_slider.getValue();
    lpf->set_q(lpf_q);
    DBG("lpf q");
}
