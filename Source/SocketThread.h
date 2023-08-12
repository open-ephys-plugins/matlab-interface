/*
------------------------------------------------------------------

This file is part of the Open Ephys GUI
Copyright (C) 2019 Open Ephys

------------------------------------------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef SOCKETTHREAD_H_INCLUDED
#define SOCKETTHREAD_H_INCLUDED

//#include "EventQueue.h"
#include "MatlabSocket.h"
#include "DataQueue.h"
#include "EventQueue.h"
#include <atomic>

#define BLOCK_MAX_WRITE_SAMPLES 4096
#define BLOCK_MAX_WRITE_EVENTS 32
#define BLOCK_MAX_WRITE_SPIKES 32

class MatlabSocket;

class SocketThread : public Thread
{
public:

	/** Constructor */
	//SocketThread();

	SocketThread(MatlabInterface * parentNode);

	/** Destructor */
	~SocketThread();

	void setQueuePointers(DataQueue * data, EventMsgQueue * events, SpikeMsgQueue * spikes);
	void setSelectedChannel(int channel);

	int openSocket(int port, String host); 

	void run() override;

	void setFirstBlockFlag(bool state);
	void forceCloseFiles();

	std::unique_ptr<MatlabSocket> socket;

	MatlabInterface *matlabInterface;

private:
	void writeHeader();
	void writeData(const AudioSampleBuffer& buffer, int maxSamples, int maxEvents, int maxSpikes, bool lastBlock = false);

	Array<int> m_channelArray;

	DataQueue* m_dataQueue;
	EventMsgQueue* m_eventQueue;
	SpikeMsgQueue *m_spikeQueue;

	std::atomic<bool> m_receivedFirstBlock;
	std::atomic<bool> m_cleanExit;

	File m_rootFolder;
	int m_experimentNumber;
	int m_recordingNumber;
	int m_numChannels;

	int channels[128] = {0};
	uint16 ids[128] = {0};
	int64 sampleIdxs[128] = {0};

	//RecordNode *recordNode;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SocketThread);
};

#endif  // RECORDTHREAD_H_INCLUDED
