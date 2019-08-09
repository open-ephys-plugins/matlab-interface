#ifndef MATLABENGINE_H_DEFINED
#define MATLABENGINE_H_DEFINED

#include <ProcessorHeaders.h>
#include "MatlabEngineEditor.h"

#include "MatlabDataArray.hpp"
#include "MatlabEngine.hpp"
#include <iostream>

#include <chrono>
#include <functional>

#define MAX_MESSAGE_SIZE 2000

template <typename Time = std::chrono::microseconds, typename Clock = std::chrono::high_resolution_clock>
struct func_timer
{
	template <typename F, typename... Args>
	static Time duration(F&& f, Args... args)
	{
		auto start = Clock::now();
		std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
		auto end = Clock::now();

		return std::chrono::duration_cast<Time>(end-start);
	} 
};

class MatlabEngine : public GenericProcessor
{
public:
	MatlabEngine();
	~MatlabEngine();

	bool hasEditor() const { return true; }
	AudioProcessorEditor* createEditor() override;

	void process(AudioSampleBuffer& buffer) override;

	void handleEvent(const EventChannel* eventInfo, const MidiMessage& event, int samplePosition) override;

	void setParameter(int parameterIndex, float newValue) override;

	/** Saving custom settings to XML. */
	//void saveCustomParametersToXml(XmlElement* parentElement) override;

	/** Load custom settings from XML*/
	//void loadCustomParametersFromXml() override;

	void updateSettings() override;

	void startMatlab();
	void runTest();
	void openSocket();

private:

	std::unique_ptr<matlab::engine::MATLABEngine> matlab;

	matlab::data::ArrayFactory factory;

	std::vector<float> matlab_data;

	int count;

};

//This class acts as a server to stream incoming Neuropixel data to Matlab
//A socket is one end of an interprocess communication channel...
class MatlabSocket : public StreamingSocket
{
public:
	MatlabSocket();
	~MatlabSocket();
private:
	ScopedPointer<StreamingSocket> connection;
	int port;
	bool connected; //true if client (Matlab) has connected
};

#endif