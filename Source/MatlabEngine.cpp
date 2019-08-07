#include "MatlabEngine.h"

MatlabEngine::MatlabEngine() : GenericProcessor("Matlab Engine")
{
	setProcessorType(PROCESSOR_TYPE_SINK);

	startMATLAB();

}

MatlabEngine::~MatlabEngine()
{

}

void MatlabEngine::startMATLAB()
{
	if (matlab == nullptr)
	{
		matlab = matlab::engine::startMATLAB();
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

