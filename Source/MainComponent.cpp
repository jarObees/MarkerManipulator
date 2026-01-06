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

void MainComponent::updateTextEditors()
{
    // Verify whatever is input into offsetInput
    int offset = 0;
    if (!offsetInput.isEmpty())
    {
        juce::String uInput = offsetInput.getText();
        bool isNegative = false;

        // Handling negative inputs
        if (uInput.startsWith("-"))
        {
            isNegative = true;
            uInput = uInput.trimCharactersAtStart("-");
        }

        auto timeInSecs = markerManager.parseTimeToSecs(uInput);
        if (timeInSecs.has_value())
        {
            offset = static_cast<int>(std::round(timeInSecs.value()));
            if (isNegative)
                offset = offset * -1;
        }
        else
        {
            DBG("Could not parse input!");
            return;
        }
    }

    // Go through all the markers and build up the text. 
    juce::String YT_TimeStamps = "";
    juce::String FS_TimeStamps = "";

    for (auto marker : markerManager.getMarkers())
    {
        double newStartTime = marker.startTimeS + offset;
        if (newStartTime < 0)
            newStartTime = 0.0;

        juce::String newMarkerStartTS = markerManager.parseSecsToTime(newStartTime);

        // YT Entry: "name: HH:MM:SS"
        YT_TimeStamps << newMarkerStartTS;
        YT_TimeStamps << " ";
        YT_TimeStamps << marker.name;

        // FS Entry: "#HH:MM:SS - name"
        FS_TimeStamps << '#';
        FS_TimeStamps << newMarkerStartTS;
        FS_TimeStamps << " - ";
        FS_TimeStamps << marker.name;

        // After every marker, make a new line
        YT_TimeStamps << '\n';
        FS_TimeStamps << '\n';
    }

    YTFormat.setText(YT_TimeStamps);
    FSFormat.setText(FS_TimeStamps);

}
void MainComponent::textEditorTextChanged(juce::TextEditor& editor)
{

}

void MainComponent::textEditorFocusLost(juce::TextEditor& editor)
{

}

void MainComponent::textEditorReturnKeyPressed(juce::TextEditor& editor)
{
    if (&editor == &offsetInput)
    {
        updateTextEditors();
    }
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
        updateTextEditors();
        repaint();
    }
}