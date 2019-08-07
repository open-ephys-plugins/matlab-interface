#ifndef MATLABENGINE_H_DEFINED
#define MATLABENGINE_H_DEFINED

#include <ProcessorHeaders.h>
#include "MatlabEngineEditor.h"

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

};

#endif