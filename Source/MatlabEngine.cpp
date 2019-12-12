#include "MatlabEngine.h"

MatlabEngine::MatlabEngine() 
	: GenericProcessor("Matlab Engine"),
	dataQueue(new DataQueue(WRITE_BLOCK_LENGTH, DATA_BUFFER_NBLOCKS)),
	socketThread(new SocketThread())
{
	setProcessorType(PROCESSOR_TYPE_FILTER);
}

MatlabEngine::~MatlabEngine()
{

}

/* Matlab engine methods for periodically calling a Matlab function on downsampled data */
int MatlabEngine::connect()
{
	return socketThread->openSocket();
}

void MatlabEngine::setSelectedChannel(int channel)
{
	selectedChannel = channel;
	socketThread->setSelectedChannel(channel);
}

void MatlabEngine::runTest()
{

	matlab::data::TypedArray<double> const argArray = factory.createArray({1,4}, {-2.0, 2.0, 6.0, 8.0 });

	matlab::data::Array const results = matlab->feval("sqrt", argArray);

	for (int i = 0; i < results.getNumberOfElements(); i++) {
		double a = argArray[i];
		std::complex<double> v = results[i];
		double realPart = v.real();
		double imgPart = v.imag();
		//std::cout << "Square root of " << a << " is " << realPart << " + " << imgPart << "i" << std::endl;
	}

}

AudioProcessorEditor* MatlabEngine::createEditor()
{
    editor = new MatlabEngineEditor (this, true);
    return editor;
}

void MatlabEngine::process(AudioSampleBuffer& buffer)
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



/* Sample call

	matlab::data::ArrayFactory factory;

	matlab::data::TypedArray<double> const argArray = factory.createArray({1,4}, {-2.0, 2.0, 6.0, 8.0 });

	matlab::data::Array const results = matlab->feval("sqrt", argArray);

	for (int i = 0; i < results.getNumberOfElements(); i++) {
		double a = argArray[i];
		std::complex<double> v = results[i];
		double realPart = v.real();
		double imgPart = v.imag();
		std::cout << "Square root of " << a << " is " << realPart << " + " << imgPart << "i" << std::endl;
	}

*/

