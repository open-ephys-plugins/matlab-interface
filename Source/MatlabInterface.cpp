#include "MatlabInterface.h"

MatlabInterface::MatlabInterface() 
	: GenericProcessor("Matlab Interface"),
	dataQueue(std::make_unique<DataQueue>(WRITE_BLOCK_LENGTH, DATA_BUFFER_NBLOCKS)),
	socketThread(std::make_unique<SocketThread>()),
	connected(false),
	selectedChannel(0)
{
	setProcessorType(Plugin::Processor::FILTER);

	addStringParameter(Parameter::GLOBAL_SCOPE, "host_address", "Set host address", "127.0.0.1", true);
    addStringParameter(Parameter::GLOBAL_SCOPE, "port_number", "Set port number", "1234", true);
	addSelectedChannelsParameter(Parameter::STREAM_SCOPE, "selected_channel", "The continuous channel to analyze", 1);

}

MatlabInterface::~MatlabInterface()
{
	if (!connected && socketThread->socket->connection != nullptr)
		socketThread->socket->connection->close();

	socketThread->signalThreadShouldExit();
	socketThread->waitForThreadToExit(2000);
}

int MatlabInterface::connect()
{
	String port = getParameter("port_number")->getValue();
	String host = getParameter("host_address")->getValue();

	if (socketThread->openSocket(port.getIntValue(), host) > 0)
		connected = true;
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

	if (!connected)
		return;

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

	for (auto stream : dataStreams)
	{
		const uint16 streamId = stream->getStreamId();
		uint32 numSamples = getNumSamplesInBlock(streamId);
		int64 sampleNumber = getFirstSampleNumberForBlock(streamId);

		for (auto channel : stream->getContinuousChannels())
			dataQueue->writeChannel(buffer, channel->getGlobalIndex(), numSamples, sampleNumber);
	}

}

void MatlabInterface::handleTTLEvent(TTLEventPtr event)
{
    //TODO
}

void MatlabInterface::updateSettings()
{
	for (auto stream : getDataStreams())
        parameterValueChanged(stream->getParameter("selected_channel"));
}
