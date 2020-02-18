/*******************************************************
*    This is an audio ingest/outgest station with      *
*    full sample rate selection, along with an input   *
*    visualizer biased towards a brown noise spectrum. *
*******************************************************/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

class AudioIngestOutgestApplication  : public JUCEApplication
{
public:
    AudioIngestOutgestApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    void initialise (const String& commandLine) override
    {
        mainWindow.reset (new MainWindow (getApplicationName()));
    }

    void shutdown() override
    {
        // Quick and dirty shutdown procedure
        mainWindow = nullptr;
    }

    void systemRequestedQuit() override
    {
        //Making sure OS can close out our program if needed
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {

    }

    class MainWindow    : public DocumentWindow {
    public:
        MainWindow (String name)  : DocumentWindow 
        (name,Desktop::getInstance().getDefaultLookAndFeel()
        .findColour (ResizableWindow::backgroundColourId),
        DocumentWindow::allButtons) {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent(), true);


           //Included JUCE use cases for mobile
           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
           #endif
            setVisible (true);
        }

        void closeButtonPressed() override {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION (AudioIngestOutgestApplication)
