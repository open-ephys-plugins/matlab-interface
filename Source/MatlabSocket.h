#include <JuceHeader.h>

#define WRITE_BUFFER_SIZE 40960
#define READ_BUFFER_SIZE 1024

//A juce::StreamingSocket class optimized for streaming data to Matlab
class MatlabSocket : public StreamingSocket
{
public:
	MatlabSocket();
	~MatlabSocket();
	int listen();
	int writeHeader();
	int writeData(int channel, const float* buffer, int size, int idx);
private:
	bool sentHeader;
	int bufferSize;
	ScopedPointer<StreamingSocket> connection;
	int port;
	bool connected; //true if client (Matlab) has connected
	char writeBuffer[WRITE_BUFFER_SIZE];
	char readBuffer[READ_BUFFER_SIZE];
};