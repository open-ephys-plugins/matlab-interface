#include "MatlabSocket.h"
#include "../../Source/CoreServices.h"
//#include "../../Source/Processors/Settings/InfoObject.h"

MatlabSocket::MatlabSocket() : port(1234)
{
	bufferSize = WRITE_BUFFER_SIZE;
	sentHeader = false;
}

MatlabSocket::~MatlabSocket()
{
	if (connection != nullptr)
	{
		connection->close();
	}
}

int MatlabSocket::listen(int port, String host)
{
	
	int rc; 

	connection = new StreamingSocket();
	connection->createListener(port); //empty string means use localhost address (127.0.0.1)
	
	if (connection->waitUntilReady(true, 500) < 1)
	{
		LOGC("Could not connect.");
		connection = nullptr;
		return -2;
	}

	LOGD("Waiting for next connection..."); 
	socket = connection->waitForNextConnection();
	
	if (socket != nullptr)
	{
		LOGD("Socket is ", (socket->isConnected() ? "connected!" : " NOT connected! "));
		bool readyForReading = false; //this means ready for writing...
		int timeout = 5000; 
		rc = socket->waitUntilReady(readyForReading, timeout);
		if (rc == 1)
		{
			LOGC("Matlab socket is ready for writing!");
		}
		else if (rc == 0)
		{
			LOGC("Matlab socket timed out!");
		}
		else
		{
			LOGD("Error occured while waiting for socket!");
		}

		if (connection->isConnected())
			connection->close();

		return rc;

	}

	return -2;

}

int MatlabSocket::writeHeader()
{


	return 1;
}

int MatlabSocket::writeData(int channel, const float* buffer, int size, int idx)
{

	if (size > bufferSize)
	{
		LOGD("Write buffer overrun, resizing from: ", bufferSize, " to: ", size);
		//TODO: Resize internal buffer?
		bufferSize = size;
	}

	if (channel == selectedChannel) //TODO: Strean as many channels as possible
	{
		
		for (int i = 0; i < size; i++)
		{
			snprintf(writeBuffer, sizeof(writeBuffer), "%f", *(buffer+i));
			LOGD("writeBuffer= ",writeBuffer);
			socket->write(writeBuffer, strlen(writeBuffer));
			socket->write(" ",strlen(" "));
		}
		socket->write("\n", strlen("\n"));

		bool blockUntilSpecifiedAmountHasArrived = false;
		//std::cout << "Waiting to receive data..." << std::endl; fflush(stdout);
		socket->read(readBuffer, READ_BUFFER_SIZE, blockUntilSpecifiedAmountHasArrived);

	}

	return 1;

}

int MatlabSocket::readData()
{

	//TODO: Process data coming from Matlab
	/*
		if (String(readBuffer) == "Stop!")
			CoreServices::setAcquisitionStatus(false);
	*/

	//Parse read buffer: You know if you see 2 space characters in a row, the rest will be spaces...

	String resp = String(readBuffer);
	int idx = 0;
	int spaceCount = 0;
	while (spaceCount < 2)
	{
		if (readBuffer[idx] == 0x20)
		{
			spaceCount++;
		}
		else
		{
			spaceCount = 0;
		}
		idx++;
	}
	// std::cout << "$" << resp.substring(0, idx) << std::endl;

	StringArray tokens;
	tokens.addTokens(resp.substring(0, idx), " ", "\"");

	// std::cout << "Got " << tokens.size() << " values!" << std::endl;
	for (int i = 0; i < tokens.size(); i++)
	{
		std::cout << tokens[i].getIntValue() << " ";
	}
	std::cout << std::endl;
	fflush(stdout);
	return 1;
}


int MatlabSocket::writeSpike(int channels[], uint16 ids[], int64 sampleIdxs[], int nSpikes) //write spike data via socket
{
	int totalSamples =sizeof(channels)+sizeof(ids);
	if (totalSamples > bufferSize) //Shouldn't happen, and if it happens it'll be slow, but better this than crashing. Will be reset on file close and reset.
	{
		LOGD("(spike) Write buffer overrun, resizing to", totalSamples);
		bufferSize = totalSamples;
	}

	
	for (int i = 0; i < nSpikes; i++) {
		snprintf(t_writeBuffer, sizeof(t_writeBuffer), "%02d+%02d ", channels[i],ids[i]);
		//example: "01+04 ;" represents a spike (id=4,electrode=1)
		//example: "01+02 02+03 01+02 ;" represents 2 spikes (id=2,elec=1) and 1 spike (id=3,elec=2) 
		
		for (int j = 0; j < strlen(t_writeBuffer); j++) {
			writeBuffer[j + writeBufferPointer] = t_writeBuffer[j];
		}
		writeBufferPointer += strlen(t_writeBuffer); //the buffer to send to socket
		
	}

	memset(readBuffer, 0, sizeof(readBuffer));
	bool blockUntilSpecifiedAmountHasArrived = false;	
	socket->read(readBuffer, READ_BUFFER_SIZE, blockUntilSpecifiedAmountHasArrived); 

	if (readBuffer[0]=='F') { //only send spike writebuffer when receiving specific command
		socket->write(writeBuffer, strlen(writeBuffer));
		socket->write("*\n", strlen("*\n")); //a marker representing the end of spike sequence, not necessary

		writeBufferPointer = 0;
		memset(writeBuffer, 0, sizeof(writeBuffer));
	}
	
	return 1;

}
