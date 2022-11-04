#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1200, 700);

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
    
    juce::AudioDeviceManager::AudioDeviceSetup currentAudioSetup;
    deviceManager.getAudioDeviceSetup (currentAudioSetup);
    currentAudioSetup.bufferSize = 64;
    deviceManager.setAudioDeviceSetup (currentAudioSetup, true);
    
    // initialise UI
    addAndMakeVisible(delay_label);
    delay_label.setFont(juce::Font (14.0f, juce::Font::bold));
    delay_label.setText("Delay", juce::dontSendNotification);
    delay_label.setColour(juce::Label::textColourId, juce::Colour(255, 255, 255));
    
    addAndMakeVisible(dry_slider);
    dry_slider.setRange(0., 1., 0.01);
    dry_slider.setValue(1.0);
    dry_slider.setTextValueSuffix(" dry");
    dry_slider.onValueChange = [this] { changeDry(); };
    
    addAndMakeVisible(wet_slider);
    wet_slider.setRange(0., 1., 0.01);
    wet_slider.setValue(0.0);
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
    
    addAndMakeVisible(lpf_freq_slider);
    lpf_freq_slider.setRange(10., 5000., 1.);
    lpf_freq_slider.setValue(500);
    lpf_freq_slider.setTextValueSuffix(" Hz");
    lpf_freq_slider.onValueChange = [this] { changeLPFfreq(); };
    
    addAndMakeVisible(lpf_q_slider);
    lpf_q_slider.setRange(0.1, 2., 0.1);
    lpf_q_slider.setValue(1);
    lpf_q_slider.setTextValueSuffix(" Q");
    lpf_q_slider.onValueChange = [this] { changeLPFq(); };
    
    addAndMakeVisible(dist_label);
    dist_label.setFont(juce::Font (14.0f, juce::Font::bold));
    dist_label.setText("Distortion", juce::dontSendNotification);
    dist_label.setColour(juce::Label::textColourId, juce::Colour(255, 255, 255));
    
    addAndMakeVisible(dist_slider);
    dist_slider.setRange(0., 1., 0.01);
    dist_slider.setValue(0.5);
    dist_slider.setTextValueSuffix(" dist");
    dist_slider.onValueChange = [this] { changeDistRate(); };
    
    addAndMakeVisible(chorus_label);
    chorus_label.setFont(juce::Font (14.0f, juce::Font::bold));
    chorus_label.setText("Chorus", juce::dontSendNotification);
    chorus_label.setColour(juce::Label::textColourId, juce::Colour(255, 255, 255));
    
    addAndMakeVisible(depth_slider);
    depth_slider.setRange(0., 1., 0.01);
    depth_slider.setValue(0.03);
    depth_slider.setTextValueSuffix(" depth");
    depth_slider.onValueChange = [this] { changeDepth(); };
    
    addAndMakeVisible(speed_slider);
    speed_slider.setRange(0., 10., 0.01);
    speed_slider.setValue(3.0);
    speed_slider.setTextValueSuffix(" speed");
    speed_slider.onValueChange = [this] { changeSpeed(); };
    
    addAndMakeVisible(reverb_label);
    reverb_label.setFont(juce::Font (14.0f, juce::Font::bold));
    reverb_label.setText("Reverb", juce::dontSendNotification);
    reverb_label.setColour(juce::Label::textColourId, juce::Colour(255, 255, 255));
    
    addAndMakeVisible(room_slider);
    room_slider.setRange(0., 1., 0.01);
    room_slider.setValue(0.03);
    room_slider.setTextValueSuffix(" room");
    room_slider.onValueChange = [this] { changeRoom(); };
    
    addAndMakeVisible(reverb_lpf_slider);
    reverb_lpf_slider.setRange(0., 1., 0.01);
    reverb_lpf_slider.setValue(3.0);
    reverb_lpf_slider.setTextValueSuffix(" lpf");
    reverb_lpf_slider.onValueChange = [this] { changeReverbLPF(); };
    
    addAndMakeVisible(reverb_wet_slider);
    reverb_wet_slider.setRange(0., 1., 0.01);
    reverb_wet_slider.setValue(0.3);
    reverb_wet_slider.setTextValueSuffix(" wet");
    reverb_wet_slider.onValueChange = [this] { changeReverbWet(); };
    
    addAndMakeVisible(reverb_dry_slider);
    reverb_dry_slider.setRange(0., 1., 0.01);
    reverb_dry_slider.setValue(0.3);
    reverb_dry_slider.setTextValueSuffix(" dry");
    reverb_dry_slider.onValueChange = [this] { changeReverbDry(); };
    
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
    delay = new MonoLPFDelay(sample_rate);
    dist = new ClippingDistortion(dist_rate);
    reverb = new Reverb8Diff(sample_rate);
    chorus = new Chorus(sample_rate);
//    octDown = new OctaveDown();
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!
    
    // For more details, see the help for AudioProcessor::getNextAudioBlock()
    auto* inReadBuffer = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);
    auto* leftWriteBuffer  = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
    auto* rightWriteBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
    for (auto i = 0; i < bufferToFill.numSamples; i++){
        float s = inReadBuffer[i];
//        s = dist->process_sample(s);
//        s = octDown->process_sample(s);
//        s = chorus->process_sample(s);
//        s = delay->process_sample(s);
        s = reverb->process_sample(s);
        leftWriteBuffer[i] = s;
        rightWriteBuffer[i] = s;
//        leftWriteBuffer[i] = inReadBuffer[i];
//        rightWriteBuffer[i] = inReadBuffer[i];
    }
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
    
    x_size = getWidth()/2. - 20;
    // x_offset = getWidth()/5. + 10;
    x_offset = 10.;
    float tmp_y = y_offset;
    
    dist_label.setBounds(      x_offset, tmp_y,                x_size, 30);
    dist_slider.setBounds(     x_offset, tmp_y += y_step,      x_size, 30);
    
    delay_label.setBounds(      x_offset, tmp_y += y_step,      x_size, 30);
    wet_slider.setBounds(       x_offset, tmp_y += y_step,      x_size, 30);
    dry_slider.setBounds(       x_offset, tmp_y += y_step,      x_size, 30);
    time_slider.setBounds(      x_offset, tmp_y += y_step,      x_size, 30);
    feedback_slider.setBounds(  x_offset, tmp_y += y_step,      x_size, 30);
    lpf_freq_slider.setBounds(  x_offset, tmp_y += y_step,      x_size, 30);
    lpf_q_slider.setBounds(     x_offset, tmp_y += y_step,      x_size, 30);
    
    chorus_label.setBounds(      x_offset, tmp_y += y_step,      x_size, 30);
    depth_slider.setBounds(      x_offset, tmp_y += y_step,      x_size, 30);
    speed_slider.setBounds(      x_offset, tmp_y += y_step,      x_size, 30);
    
    reverb_label.setBounds(      x_offset, tmp_y += y_step,      x_size, 30);
    room_slider.setBounds(       x_offset, tmp_y += y_step,      x_size, 30);
    reverb_lpf_slider.setBounds( x_offset, tmp_y += y_step,      x_size, 30);
    reverb_wet_slider.setBounds( x_offset, tmp_y += y_step,      x_size, 30);
    reverb_dry_slider.setBounds( x_offset, tmp_y += y_step,      x_size, 30);
    
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
    delay->setLPFfrequency(lpf_freq);
    DBG("lpf frequency");
}

void MainComponent::changeLPFq(){
    lpf_q = lpf_q_slider.getValue();
    delay->setLPFq(lpf_q);
    DBG("lpf q");
}

void MainComponent::changeDepth(){
    depth = depth_slider.getValue();
    chorus->setDepth(depth);
    DBG("depth");
}
void MainComponent::changeSpeed(){
    speed = speed_slider.getValue();
    chorus->setSpeed(speed);
    DBG("speed");
}

void MainComponent::changeRoom(){
    reverb->set_room_size(room_slider.getValue());
    DBG("reverb room");
}

void MainComponent::changeReverbLPF(){
    reverb->set_lpf(reverb_lpf_slider.getValue());
    DBG("rev lpf");
}
void MainComponent::changeReverbWet(){
    reverb->set_wet(reverb_wet_slider.getValue());
    DBG("rev wet");
}
void MainComponent::changeReverbDry(){
    reverb->set_dry(reverb_dry_slider.getValue());
    DBG("rev dry");
}
