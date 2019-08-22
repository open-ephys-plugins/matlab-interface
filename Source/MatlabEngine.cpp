#include "MatlabEngine.h"

MatlabSocket::MatlabSocket() : StreamingSocket(), port(1234)
{

	char buffer[MAX_MESSAGE_SIZE];
	char stream[] = "TEST STREAM\n";

	createListener(port,""); //empty string means use localhost address (127.0.0.1)
	std::cout << "Waiting for next connection...\n" << std::endl; fflush(stdout);
	connection = waitForNextConnection();
	if (connection != nullptr)
	{
		std::cout << "Socket is connected!" << std::endl;
		std::cout << "Waiting until socket is ready for writing..." << std::endl;
		bool readyForReading = false; //this means ready for writing...
		int timeout = 3000; 
		int rc = connection->waitUntilReady(readyForReading, timeout);
		if (rc == 1)
			std::cout << "Socket is ready for writing!" << std::endl;
		else if (rc == 0)
			std::cout << "Socket timed out!" << std::endl;
		else
			std::cout << "Error occured while waiting for socket!" << std::endl;

		std::chrono::milliseconds t = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch());
		long long t_s = t.count()*std::chrono::milliseconds::period::num / std::chrono::milliseconds::period::den;

		int count = 0;
		while (count < 10000)
		{

			t = std::chrono::duration_cast< std::chrono::milliseconds >(
			std::chrono::system_clock::now().time_since_epoch());
			long long t_n = t.count()*std::chrono::nanoseconds::period::num / std::chrono::seconds::period::den;

			if (t_n > t_s)
			{
				count++;
				t_s = t_n;
				//Send 60 bytes every 1 second.
				connection->write(stream, strlen(stream));
				//std::cout << count <<  ": Wrote stream w/ length: " << strlen(stream) << std::endl;
			}
			
		}
		char EOM[] = "~";
		connection->write(EOM, strlen(EOM));
		return;
	}
	printf("Socket timed out!\n");

}

MatlabSocket::~MatlabSocket()
{
	if (connection != nullptr)
		connection->close();
}

MatlabEngine::MatlabEngine() : GenericProcessor("Matlab Engine")
{
	setProcessorType(PROCESSOR_TYPE_SINK);
}

MatlabEngine::~MatlabEngine()
{

}

void MatlabEngine::startMatlab()
{
	if (matlab == nullptr)
	{
		matlab = matlab::engine::startMATLAB();
	}
	else
	{
		printf("MATLAB has already started\n");                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	}
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

void MatlabEngine::openSocket()
{
	ScopedPointer<MatlabSocket> socket = new MatlabSocket();
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
	const int nSamples = buffer.getNumSamples();
	const int size = nChannels * nSamples; 

	std::vector<float> matlab_data;
	size_t dataSize = 1024*768*1;
	matlab_data.reserve(dataSize);

	for (int ch = 0; ch < nChannels; ++ch)
	{
		float* bufPtr = buffer.getWritePointer(ch);
		for (int n = 0; n < nSamples; ++n)
		{
			matlab_data.push_back(*(bufPtr + n));
		}
	}

	/*
	matlab::data::TypedArray<float> argArray = factory.createArray({1,dataSize}, std::begin(matlab_data), std::end(matlab_data));
	matlab->setVariable("data", std::move(argArray));
	matlab::data::Array results = matlab->feval("sqrt", argArray);
	*/
	 
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

