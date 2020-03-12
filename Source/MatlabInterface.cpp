#include "MatlabInterface.h"

MatlabInterface::MatlabInterface() 
	: GenericProcessor("Matlab Interface"),
	dataQueue(new DataQueue(WRITE_BLOCK_LENGTH, DATA_BUFFER_NBLOCKS)),
	socketThread(new SocketThread())
{
	setProcessorType(PROCESSOR_TYPE_FILTER);
}

MatlabInterface::~MatlabInterface()
{

}

int MatlabInterface::connect(String port, String host)
{
	//TODO: Basic host and port error checking
	return socketThread->openSocket(port.getIntValue(), host);
}

void MatlabInterface::setSelectedChannel(int channel)
{
	selectedChannel = channel;
	socketThread->setSelectedChannel(channel);
}

AudioProcessorEditor* MatlabInterface::createEditor()
{
    editor = new MatlabInterfaceEditor (this, true);
    return editor;
}

void MatlabInterface::process(AudioSampleBuffer& buffer)
{

	//checkForEvents(false);
	const int nChannels = buffer.getNumChannels();

	if (!socketThread->isThreadRunning())
	{
		dataQueue->setChannels(nChannels);
		socketThread->setQueuePointers(dataQueue);
		socketThread->setFirstBlockFlag(true);
		socketThread->setSelectedChannel(selectedChannel);
		socketThread->startThread();
	}

	for (int ch = 0; ch < nChannels; ++ch)
	{
		int numSamples = getNumSamples(ch);
		int timestamp = getTimestamp(ch);
		dataQueue->writeChannel(buffer, ch, numSamples, timestamp);
	}

}

void MatlabInterface::handleEvent(const EventChannel* eventInfo, const MidiMessage& event, int samplePosition)
{
    //TODO
}

void MatlabInterface::setParameter (int parameterIndex, float newValue)
{
    //TODO
}

void MatlabInterface::updateSettings()
{
	//TODO
}
