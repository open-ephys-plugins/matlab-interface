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

#include "MatlabInterface.h"
#include "SocketThread.h"


SocketThread::SocketThread(MatlabInterface* parentNode) :
Thread("Socket Thread"),
m_receivedFirstBlock(false),
m_cleanExit(true),
matlabInterface(parentNode)
{
	socket = std::make_unique<MatlabSocket>();
}

SocketThread::~SocketThread()
{
}

void SocketThread::setQueuePointers(DataQueue* data, EventMsgQueue* events, SpikeMsgQueue* spikes)
{
	m_dataQueue = data;
	m_eventQueue = events;
	m_spikeQueue = spikes;
}

void SocketThread::setSelectedChannel(int channel)
{
	socket->selectedChannel = channel;
}

int SocketThread::openSocket(int port, String host)
{
	LOGC("Opening socket...");
	if (socket->listen(port, host) == 1)
	{
		LOGC("Writing header...");
		socket->writeHeader();

		return 1;
	}
	else {
		return 0;
	}
	
	
}

void SocketThread::setFirstBlockFlag(bool didReceiveFirstBlock)
{
	m_receivedFirstBlock = didReceiveFirstBlock;
	this->notify(); //breaks the thread out of the wait method 
}

void SocketThread::run()
{
	//LOGD("socketthread::run");

	const AudioSampleBuffer& dataBuffer = m_dataQueue->getAudioBufferReference();
	bool closeEarly = true;


	//1-Wait until the first block has arrived, so we can align the timestamps
	while (!m_receivedFirstBlock && !threadShouldExit())
	{
		printf("\rWaiting for first block..."); fflush(stdout);
		wait(1);
	}

	//3-Normal loop
	while (!threadShouldExit())
	{
		writeData(dataBuffer, BLOCK_MAX_WRITE_SAMPLES, BLOCK_MAX_WRITE_EVENTS, BLOCK_MAX_WRITE_SPIKES, false);
	}

	//4-Before closing the thread, try to write the remaining samples
	if (!closeEarly)
	{
		writeData(dataBuffer, -1, -1, -1, true);
	}
	m_cleanExit = true;
	m_receivedFirstBlock = false;

}

void SocketThread::writeData(const AudioSampleBuffer& dataBuffer, int maxSamples, int maxEvents, int maxSpikes, bool lastBlock)
{
	//stream continuous data, disabled
	Array<int64> timestamps;
	Array<CircularBufferIndexes> idx;
	m_dataQueue->startRead(idx, timestamps, maxSamples);
	m_numChannels = dataBuffer.getNumChannels();

	for (int chan = 0; chan < m_numChannels; ++chan)
	{
		if (idx[chan].size1 > 0)
		{
			//socket->writeData(chan, dataBuffer.getReadPointer(chan, idx[chan].index1), idx[chan].size1, 1);
			if (idx[chan].size2 > 0)
			{
				timestamps.set(chan, timestamps[chan] + idx[chan].size1);
				//socket->writeData(chan, dataBuffer.getReadPointer(chan, idx[chan].index2), idx[chan].size2, 2);
			}
		}
	}
	m_dataQueue->stopRead(); //to prevent memory overflow
	

	//stream spike data, enabled
	std::vector<SpikeMessagePtr> spikes;
	int nSpikes = m_spikeQueue->getEvents(spikes, BLOCK_MAX_WRITE_SPIKES);

	memset(channels, 0, sizeof(channels));
	memset(ids, 0, sizeof(ids));
	memset(sampleIdxs, 0, sizeof(sampleIdxs));

	//if (nSpikes > 0) { LOGD("Nspikes = ", nSpikes); }
	for (int sp = 0; sp < nSpikes; ++sp)
	{
		
		if (spikes[sp] != nullptr)
		{

			const Spike& spike = spikes[sp]->getData();
			const SpikeChannel* chan = spike.getChannelInfo();
			int spikeIndex = matlabInterface->getIndexOfMatchingChannel(chan);

			const Spike* spikedata = &spikes[sp]->getData();
			int64 sampleIdx = spikedata->getSampleNumber();
			uint16 sortedId = spikedata->getSortedId();
			
			//LOGD("spike index = ", spikeIndex+1);
			//LOGD("sorted id = ", sortedId);

			channels[sp] = spikeIndex+1;
			ids[sp] = sortedId;
			sampleIdxs[sp] = sampleIdx;

		}
	}
	socket->writeSpike(channels, ids, sampleIdxs, nSpikes);

}
