#include "MatlabEngine.h"

//Change all names for the relevant ones, including "Processor Name"
MatlabEngine::MatlabEngine() : GenericProcessor("Matlab Engine")
{
	setProcessorType(PROCESSOR_TYPE_SINK);
}

MatlabEngine::~MatlabEngine()
{

}

AudioProcessorEditor* MatlabEngine::createEditor()
{
    editor = new MatlabEngineEditor (this, true);
    return editor;
}

void MatlabEngine::process(AudioSampleBuffer& buffer)
{
	//checkForEvents(false);
	int numChannels = getNumOutputs();

	for (int chan = 0; chan < numChannels; chan++)
	{
		int numSamples = getNumSamples(chan);
		int64 timestamp = getTimestamp(chan);

		//Do whatever processing needed
	}
	 
}

void MatlabEngine::handleEvent(const EventChannel* eventInfo, const MidiMessage& event, int samplePosition)
{
    //TODO
}

void MatlabEngine::setParameter (int parameterIndex, float newValue)
{
    //TODO
}

void MatlabEngine::updateSettings()
{
	//TODO
}

