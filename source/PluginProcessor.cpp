#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "bridge.h"
#include "Api.h"
#include "libraryloader.h"
#include "EditorHelper.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output 1", juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output 2", juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output 3", juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output 4", juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output 5", juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output 6", juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output 7", juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output 8", juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output 9", juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output 10", juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output 11", juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output 12", juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output 13", juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output 14", juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output 15", juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output 16", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
     m_editorHelper(new OpenVpi::EditorHelper(this)),
     m_bridge(new OpenVpi::Bridge(m_editorHelper.get()))
{
    m_bridge->initialize();
    std::cerr << "Initialized: Processor" << std::endl;
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
    //TODO terminate
    std::cerr << "Will finalize: Processor" << std::endl;
    m_bridge->terminate();
    std::cerr << "Finalized: Processor" << std::endl;
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);
    m_bridge->setupProcess(getTotalNumOutputChannels(), sampleRate, samplesPerBlock);
    std::cerr << "Processor: prepare to play" << std::endl;
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    std::cerr << "Processor: stop playing" << std::endl;
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    auto position = getPlayHead()->getPosition();
    int64_t timeInSamples = 0;
    bool isPlaying = false;
    bool isRealtime = true;
    if(position.hasValue()) {
        timeInSamples = position->getTimeInSamples().orFallback(0);
        isPlaying = position->getIsPlaying() || position->getIsRecording();
        isRealtime = !isNonRealtime();
    }

    juce::ScopedNoDenormals noDenormals;
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
//    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
//        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    if(!m_bridge->processPlayback(buffer, timeInSamples, isRealtime, isPlaying)) {
        buffer.clear();
    }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    int size = 0;
    const char *data = nullptr;
    m_bridge->saveData(size, data);
    if(size != 0) destData.replaceAll(data, size);
    m_bridge->freeDataBuffer(data);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    m_bridge->loadData(sizeInBytes, (char *)data);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}
