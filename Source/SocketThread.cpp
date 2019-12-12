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

#include "SocketThread.h"

SocketThread::SocketThread() :
Thread("Socket Thread"),
m_receivedFirstBlock(false),
m_cleanExit(true)
{
	socket = new MatlabSocket();
}

SocketThread::~SocketThread()
{
}

void SocketThread::setQueuePointers(DataQueue* data)
{
	m_dataQueue = data;
}

void SocketThread::setSelectedChannel(int channel)
{
	socket->selectedChannel = channel;
}

int SocketThread::openSocket()
{
	socket->listen();
	socket->writeHeader();
	return 1;
}

void SocketThread::setFirstBlockFlag(bool didReceiveFirstBlock)
{
	m_receivedFirstBlock = didReceiveFirstBlock;
	this->notify(); //breaks the thread out of the wait method 
}

void SocketThread::run()
{

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
	std::cout << "Exiting record thread" << std::endl;
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

	Array<int64> timestamps;
	Array<CircularBufferIndexes> idx;
	m_dataQueue->startRead(idx, timestamps, maxSamples);
	m_numChannels = dataBuffer.getNumChannels();
	//EVERY_ENGINE->updateTimestamps(timestamps);
	//socket->updateTimestamps(timestamps);
	//EVERY_ENGINE->startChannelBlock(lastBlock);
	//socket->startChannelBlock(lastBlock);
	for (int chan = 0; chan < m_numChannels; ++chan)
	{
		if (idx[chan].size1 > 0)
		{
			//EVERY_ENGINE->writeData(chan, m_channelArray[chan], dataBuffer.getReadPointer(chan, idx[chan].index1), idx[chan].size1);
			socket->writeData(chan, dataBuffer.getReadPointer(chan, idx[chan].index1), idx[chan].size1, 1);
			if (idx[chan].size2 > 0)
			{
				timestamps.set(chan, timestamps[chan] + idx[chan].size1);
				//EVERY_ENGINE->updateTimestamps(timestamps, chan);
				//socket->updateTimestamps(timestamps, chan);
				//EVERY_ENGINE->writeData(chan, m_channelArray[chan], dataBuffer.getReadPointer(chan, idx[chan].index2), idx[chan].size2);
				socket->writeData(chan, dataBuffer.getReadPointer(chan, idx[chan].index2), idx[chan].size2, 2);
			}
		}
	}
	m_dataQueue->stopRead();
	//EVERY_ENGINE->endChannelBlock(lastBlock);
	//socket->endChannelBlock(lastBlock);

	/*
	std::vector<EventMessagePtr> events;
	int nEvents = m_eventQueue->getEvents(events, maxEvents);
	for (int ev = 0; ev < nEvents; ++ev)
	{
		const MidiMessage& event = events[ev]->getData();
		if (SystemEvent::getBaseType(event) == SYSTEM_EVENT)
		{
			uint16 sourceID = SystemEvent::getSourceID(event);
			uint16 subProcIdx = SystemEvent::getSubProcessorIdx(event);
			int64 timestamp = SystemEvent::getTimestamp(event);
			EVERY_ENGINE->writeTimestampSyncText(sourceID, subProcIdx, timestamp,
				AccessClass::getProcessorGraph()->getRecordNode()->getSourceTimestamp(sourceID, subProcIdx),
				SystemEvent::getSyncText(event));
		}
		else
			EVERY_ENGINE->writeEvent(events[ev]->getExtra(), events[ev]->getData());
	}

	std::vector<SpikeMessagePtr> spikes;
	int nSpikes = m_spikeQueue->getEvents(spikes, maxSpikes);
	for (int sp = 0; sp < nSpikes; ++sp)
	{
		EVERY_ENGINE->writeSpike(spikes[sp]->getExtra(), &spikes[sp]->getData());
	}
	*/
}
