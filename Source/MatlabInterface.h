#ifndef MATLABINTERFACE_H_DEFINED
#define MATLABINTERFACE_H_DEFINED

#include <ProcessorHeaders.h>
#include "MatlabInterfaceEditor.h"

#include "DataQueue.h"
#include "SocketThread.h"

#include <iostream>
#include <chrono>
#include <functional>

#include <algorithm>
#include <random>
#include <vector>

#define MAX_MESSAGE_SIZE 		2000

#define WRITE_BLOCK_LENGTH		1024	
#define DATA_BUFFER_NBLOCKS		300
#define EVENT_BUFFER_NEVENTS	512
#define SPIKE_BUFFER_NSPIKES	512

#define NIDAQ_BIT_VOLTS			0.001221f
#define NPX_BIT_VOLTS			0.195f
#define MAX_BUFFER_SIZE			40960
#define CHANNELS_PER_THREAD		384


class MatlabInterface : public GenericProcessor
{
public:

	/** Constructor */
	MatlabInterface();

	/** Destructor */
	~MatlabInterface();

	/** Create custom editor */
	AudioProcessorEditor* createEditor() override;

	/** Set currently selected channel */
	void setSelectedChannel(int channel);

	/** Get currently selected channel */
	int getSelectedChannel() { return selectedChannel; };

	/** Check for Matlab instance at specified address/port */
	bool connect();

	/** Stream continuous channel to Matlab */
	void process(AudioBuffer<float>& buffer) override;

	/** Update selected channel for each stream */
	void updateSettings() override;

	void writeSpike(const Spike * spike, const SpikeChannel * spikeElectrode);

	
	/** Called whenever a parameter's value is changed (called by GenericProcessor::setParameter())*/
    void parameterValueChanged(Parameter* param) override;

	std::unique_ptr<SocketThread> socketThread;

private:

	bool connected;

	int selectedChannel;

	std::unique_ptr<DataQueue> dataQueue;
	std::unique_ptr<EventMsgQueue> eventQueue;
	std::unique_ptr<SpikeMsgQueue> spikeQueue;

	std::vector<float> matlab_data;

	void handleSpike(SpikePtr spike) override;

};

#endif