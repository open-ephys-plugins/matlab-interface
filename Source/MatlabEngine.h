#ifndef MATLABENGINE_H_DEFINED
#define MATLABENGINE_H_DEFINED

#include <ProcessorHeaders.h>
#include "MatlabEngineEditor.h"

#include "MatlabDataArray.hpp"
#include "MatlabEngine.hpp"
#include <iostream>

#include <chrono>
#include <functional>

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

private:

	std::unique_ptr<matlab::engine::MATLABEngine> matlab;

};

#endif