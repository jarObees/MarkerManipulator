#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // User Offset Input ========================================
    offsetInput.setMultiLine(false);
    offsetInput.setReturnKeyStartsNewLine(false);
    offsetInput.setTextToShowWhenEmpty("Input offset (HH::MM::SS, MM::SS, SS)", juce::Colours::grey);
    offsetInput.addListener(this);
    addAndMakeVisible(offsetInput);

    // YT Display
    YTFormat.setMultiLine(true);
    YTFormat.setReadOnly(true);
    YTFormat.setScrollbarsShown(true);
    YTFormat.setTextToShowWhenEmpty("YT Timestamp Format", juce::Colours::grey);
    addAndMakeVisible(YTFormat);

    // FreeSounds Display
    FSFormat.setMultiLine(true);
    FSFormat.setReadOnly(true);
    FSFormat.setScrollbarsShown(true);
    FSFormat.setTextToShowWhenEmpty("FS Timestamp Format", juce::Colours::grey);
    addAndMakeVisible(FSFormat);

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
    // g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    auto r = getLocalBounds().reduced(12);
    
    // Draw in the offsetInput
    auto topRow = r.removeFromTop(36);
    offsetInput.setBounds(topRow.reduced(6));

    // Draw in the rest.
    auto columns = r.removeFromTop(r.getHeight());
    auto leftSpace = columns.removeFromLeft(columns.getWidth() / 2).reduced(6);
    auto rightSpace = columns.reduced(6);

    YTFormat.setBounds(leftSpace);
    FSFormat.setBounds(rightSpace);
}

void MainComponent::textEditorTextChanged(juce::TextEditor& editor)
{

}

void MainComponent::textEditorFocusLost(juce::TextEditor& editor)
{

}

void MainComponent::textEditorReturnKeyPressed(juce::TextEditor& editor)
{

}

// Stuff for file dropping shenanigans ============================================================
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