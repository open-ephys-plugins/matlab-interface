#include <JuceHeader.h>

#define MAX_BUFFER_SIZE 40960

//A juce::StreamingSocket class optimized for streaming data to Matlab
class MatlabSocket : public StreamingSocket
{
public:
	MatlabSocket();
	~MatlabSocket();
	int listen();
	int writeData(int channel, const float* buffer, int size);
private:
	int bufferSize;
	ScopedPointer<StreamingSocket> connection;
	int port;
	bool connected; //true if client (Matlab) has connected
	char dataBuffer[MAX_BUFFER_SIZE];
};