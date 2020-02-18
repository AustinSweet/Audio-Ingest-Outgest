#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Visualiser : public AudioVisualiserComponent {
    public:
        Visualiser() : AudioVisualiserComponent(2) {
            setBufferSize(256);
            setSamplesPerBlock(256);
            setColours(Colours::grey, Colours::black);
    }
};

class MainComponent   : public AudioAppComponent,
                        public ChangeBroadcaster
{
public:
    MainComponent();
    ~MainComponent();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void paint (Graphics& g) override;    
    void resized() override;

    enum {
        fftOrder = 11,
        fftSize = 1 << fftOrder,
        scopeSize = 512
    };

private:

    //Audio Visualiser Cores
    Visualiser visualiser;
    AudioThumbnailCache thumbnailCache;
    AudioThumbnail thumbnail;

    //Managing our I/O
    AudioDeviceManager otherDeviceManager;
    std::unique_ptr<AudioDeviceSelectorComponent> audioSettings;

    //Defining our states
    enum TransportState {
        Stopped,
        Starting,
        Stopping,
        Playing
    };

    TransportState state;
    void transportStateChange(TransportState newState);
    void changeListenerCallback(ChangeBroadcaster* source);

    //Interactive Button section
    void stopButtonClicked();
    void playButtonClicked();
    void openButtonClicked();

    TextButton openButton;
    TextButton playButton;
    TextButton stopButton;

    //Audio Parameter Sources
    AudioFormatManager formatManager;
    AudioTransportSource transport;
    std::unique_ptr<AudioFormatReaderSource> playSource;

    //JUCE execution component
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
