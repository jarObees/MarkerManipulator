#pragma once

#include <JuceHeader.h>
#include "MarkerManager.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::Component, public juce::FileDragAndDropTarget,
    private juce::TextEditor::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    // File Drag and Drop ==========================================================
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

private:
    //==============================================================================
    // Your private member variables go here...
    Marker::MarkerManager markerManager;

    juce::TextEditor offsetInput;
    juce::TextEditor YTFormat;
    juce::TextEditor FSFormat;

    void textEditorTextChanged(juce::TextEditor& editor) override;
    void textEditorReturnKeyPressed(juce::TextEditor& editor) override;
    void textEditorFocusLost(juce::TextEditor& editor) override;

    void updateTextEditors();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
