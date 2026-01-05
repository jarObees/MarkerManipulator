#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::FontOptions (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

bool MainComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    return files.size() > 0 && files[0].endsWithIgnoreCase(".txt");
}

void MainComponent::filesDropped(const juce::StringArray& files, int, int)
{
    juce::File file(files[0]);

    if (file.existsAsFile())
    {
        markerManager.readFile(file);

        repaint();
    }
}