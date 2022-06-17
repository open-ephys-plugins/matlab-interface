#include "MatlabSocket.h"
#include "../../Source/CoreServices.h"

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