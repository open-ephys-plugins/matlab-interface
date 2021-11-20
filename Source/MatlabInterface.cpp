#include "MatlabInterface.h"

MatlabInterface::MatlabInterface() 
	: GenericProcessor("Matlab Interface"),
	dataQueue(std::make_unique<DataQueue>(WRITE_BLOCK_LENGTH, DATA_BUFFER_NBLOCKS)),
	socketThread(std::make_unique<SocketThread>())
{
	setProcessorType(Plugin::Processor::FILTER);

	addStringParameter(Parameter::GLOBAL_SCOPE, "host_address", "Set host address", "127.0.0.1", true);
    addStringParameter(Parameter::GLOBAL_SCOPE, "port_number", "Set port number", "1234", true);
	addSelectedChannelsParameter(Parameter::STREAM_SCOPE, "selected_channel", "The continuous channel to analyze", 1);
}

MatlabInterface::~MatlabInterface()
{

}

int MatlabInterface::connect()
{
	//TODO: Basic host and port error checking
	String port = getParameter("port_number")->getValue();
	String host = getParameter("host_address")->getValue();

	return socketThread->openSocket(port.getIntValue(), host);
}

void MatlabInterface::setSelectedChannel(int channel)
{
	selectedChannel = channel;
	socketThread->setSelectedChannel(channel);
}

AudioProcessorEditor* MatlabInterface::createEditor()
{
    editor = std::make_unique<MatlabInterfaceEditor>(this);
    return editor.get();
}

void MatlabInterface::parameterValueChanged(Parameter* param)
{
	if (param->getName().equalsIgnoreCase("selected_channel"))
    {
        int localIndex = (int)param->getValue();
        int globalIndex = getDataStream(param->getStreamId())->getContinuousChannels()[localIndex]->getGlobalIndex();
		setSelectedChannel(globalIndex);
    } 

}

void MatlabInterface::process(AudioSampleBuffer& buffer)
{

	//checkForEvents(false);
	const int nChannels = buffer.getNumChannels();

	if (!socketThread->isThreadRunning())
	{
		dataQueue->setChannels(nChannels);
		socketThread->setQueuePointers(dataQueue.get());
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

void MatlabInterface::updateSettings()
{
	for (auto stream : getDataStreams())
	{
        parameterValueChanged(stream->getParameter("selected_channel"));
	}
}
