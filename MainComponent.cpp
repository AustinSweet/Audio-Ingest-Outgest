#include "MainComponent.h"

MainComponent::MainComponent() :
    juce::AudioAppComponent(otherDeviceManager),
    state(Stopped), 
    thumbnailCache(5),                           
    thumbnail(512, formatManager, thumbnailCache),
    openButton("Open"), playButton("Play"), stopButton("Stop") {

    otherDeviceManager.initialise(2, 2, nullptr, true);
    audioSettings.reset(new AudioDeviceSelectorComponent(otherDeviceManager, 2, 2, 2, 2, true, true, true, true));
    setAudioChannels(2, 2);

    setSize (400, 800);

    //Initializing our button functions in this file
    openButton.onClick = [this] {openButtonClicked();};

    playButton.onClick = [this] {playButtonClicked();};
    playButton.setColour(TextButton::buttonColourId, Colours::cornflowerblue);
    playButton.setEnabled(true);

    stopButton.onClick = [this] {stopButtonClicked();};
    stopButton.setColour(TextButton::buttonColourId, Colours::orange);
    stopButton.setEnabled(false);

    addAndMakeVisible(audioSettings.get());
    addAndMakeVisible(&visualiser);
    addAndMakeVisible(&stopButton);
    addAndMakeVisible(&playButton);
    addAndMakeVisible(&openButton);

    formatManager.registerBasicFormats();
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio)) {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else {
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent() {
    // This shuts down the audio device and clears the current audio source
    shutdownAudio();
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate) {
    //All the necessary clearing of our visual and data buffers before playback
    visualiser.clear();
    transport.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::openButtonClicked() {

    DBG("open clicked");

    //choosing file

    FileChooser fileChooser ("Select a mp3, .wav, or .aiff file", File::getSpecialLocation(File::userHomeDirectory), "*.wav; *.aiff; *.mp3");

    //If the user chooses a file, find out what was chosen, then read and prepare the file.
    //Also, set up the transport position using the new file location

    if (fileChooser.browseForFileToOpen()) {

        File userFile;
        userFile = fileChooser.getResult();

        AudioFormatReader* fileReader = formatManager.createReaderFor(userFile);

        if (fileReader != nullptr) {

            std::unique_ptr<AudioFormatReaderSource> tempSource(new AudioFormatReaderSource(fileReader, true));

            transport.setSource(tempSource.get());
            transportStateChange(Stopped);
            thumbnail.setSource(new FileInputSource(userFile));
            playSource.reset(tempSource.release());

            DBG(fileReader->getFormatName());
        }
    }
}

void MainComponent::playButtonClicked() {
    transportStateChange(Starting);
}

void MainComponent::stopButtonClicked() {
    transportStateChange(Stopping);
}

void MainComponent::transportStateChange(TransportState newState) {
    //Defining playback behavior depending on where the audio reader is positioned and what state it is in

    if (newState != state) {
        state = newState;
        
        switch (state) {
        case Stopped:
            playButton.setEnabled(true);
            transport.setPosition(0.0);
            break;

        case Starting:
            stopButton.setEnabled(true);
            playButton.setEnabled(false);
            transport.start();
            break;

        case Stopping:
            playButton.setEnabled(true);
            stopButton.setEnabled(false);
            transport.stop();
            break;

        case Playing:
            playButton.setEnabled(true);
            stopButton.setEnabled(false);
            break;
        }
    }
}

void MainComponent::changeListenerCallback(ChangeBroadcaster *source) {

    if (source == &transport) {
        if (transport.isPlaying()) {
            transportStateChange(Playing);
        }
        else {
            transportStateChange(Stopped);
        }
    }

    if (source == &thumbnail) {
        repaint(10, 500, getWidth() - 20, 120);
    }
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill){
    //Sort of the "in-between" buffer active during playback
    visualiser.pushBuffer(bufferToFill);
    bufferToFill.clearActiveBufferRegion();
    transport.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources() {
}

void drawThumbnail() {
}

void MainComponent::paint (Graphics& g) {
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));  
}

void MainComponent::resized() {
    openButton.setBounds(10, 10, getWidth() - 20, 30);
    playButton.setBounds(10, 50, getWidth() - 20, 30);
    audioSettings->setBounds(10, 130, getWidth() - 20, 100);
    stopButton.setBounds(10, 90, getWidth() - 20, 30);
    visualiser.setBounds(10, 550, getWidth() - 20, 200);
}
