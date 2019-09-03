#include "MatlabSocket.h"

MatlabSocket::MatlabSocket() : StreamingSocket(), port(1234)
{
	bufferSize = MAX_BUFFER_SIZE;

}

MatlabSocket::~MatlabSocket()
{
	if (connection != nullptr)
		connection->close();
}

int MatlabSocket::listen()
{

	int rc; 

	createListener(port,""); //empty string means use localhost address (127.0.0.1)
	std::cout << "Waiting for next connection...\n" << std::endl; fflush(stdout);
	connection = waitForNextConnection();
	if (connection != nullptr)
	{

		std::cout << "Socket is connected!" << std::endl;
		std::cout << "Waiting until socket is ready for writing..." << std::endl;
		bool readyForReading = false; //this means ready for writing...
		int timeout = 3000; 
		rc = connection->waitUntilReady(readyForReading, timeout);
		if (rc == 1)
			std::cout << "Socket is ready for writing!" << std::endl;
		else if (rc == 0)
			std::cout << "Socket timed out!" << std::endl;
		else
			std::cout << "Error occured while waiting for socket!" << std::endl;

		return rc;

	}
	printf("Socket timed out!\n");
	return -2;

}

int MatlabSocket::writeData(int channel, const float* buffer, int size)
{

	if (size > bufferSize)
	{
		std::cerr << "Write buffer overrun, resizing from: " << bufferSize << " to: " << size << std::endl;
		//TODO: Resize internal buffer?
		bufferSize = size;
	}

	if (channel == 0)
	{
		
		for (int i = 0; i < size; i++)
		{
			snprintf(dataBuffer, sizeof(dataBuffer), "%f", *(buffer+i));
			connection->write(dataBuffer, strlen(dataBuffer));
			connection->write(" ",strlen(" "));
		}
		connection->write("\n", strlen("\n"));

	}

	return 1;

}