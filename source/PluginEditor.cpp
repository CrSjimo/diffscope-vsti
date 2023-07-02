#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "bridge.h"
#include "EditorHelper.h"
#include "Api.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    mainButton.setButtonText("Show DiffScope Editor");
    mainButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0x15, 0x4c, 0x39));
    mainButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0x0f, 0x69, 0x4b));
    mainButton.addListener(this);
    mainButton.setEnabled(processorRef.m_bridge->m_api->getInitializationState());
    errorLabel.setColour(juce::Label::textColourId, juce::Colours::red);
    errorLabel.setJustificationType(juce::Justification::bottom);
    setStatus("Not Connected");
//    setError("This is a test");
    versionLabel.setText(juce::String("Version ") + JucePlugin_VersionString, juce::dontSendNotification);
    versionLabel.setJustificationType(juce::Justification::right);
    addAndMakeVisible(statusLabel);
    addAndMakeVisible(errorLabel);
    addAndMakeVisible(versionLabel);
    addAndMakeVisible(mainButton);
    setSize (400, 200);
    processorRef.m_editorHelper->setEditor(this);
    processorRef.m_bridge->showEditorWindow();
    std::cerr << "Initialized: Editor" << std::endl;
}

void AudioPluginAudioProcessorEditor::setStatus(const juce::String &status) {
    juce::MessageManagerLock locker;
    statusLabel.setText(juce::String("Status: ") + status, juce::dontSendNotification);
}

void AudioPluginAudioProcessorEditor::setError(const juce::String &error) {
    juce::MessageManagerLock locker;
    errorLabel.setText(error, juce::dontSendNotification);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    processorRef.m_editorHelper->setEditor(nullptr);
    processorRef.m_bridge->hideEditorWindow();
    std::cerr << "Finalized: Editor" << std::endl;
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g) {
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colour(0x1e, 0x1e, 0x1e));
    g.setColour(juce::Colour(0x25, 0x25, 0x26));
    g.fillRect(0, 150, 400, 50);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    mainButton.setBounds(100, 50, 200, 50);
    statusLabel.setBounds(8, 176, 384, 16);
    errorLabel.setBounds(8, 16, 384, 160);
    versionLabel.setBounds(8, 176, 384, 16);
}

void AudioPluginAudioProcessorEditor::buttonClicked(juce::Button *btn) {
    setError("");
    processorRef.m_bridge->showEditorWindow();
}
