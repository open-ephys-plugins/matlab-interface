#include <JuceHeader.h>

#define WRITE_BUFFER_SIZE 40960
#define READ_BUFFER_SIZE 1024

class MatlabInterface;

/** 
 
	A Juce StreamingSocket class optimized for streaming data to Matlab
 
 */
class MatlabSocket
{
public:

	/** Constructor */
	MatlabSocket();

	/** Destructor */
	~MatlabSocket();

	/** Listen at port and host*/
	int listen(int port, String host);

	/** Write header to connected socket */
	int writeHeader();

	/** Write outgoin data for one channel */
	int writeData(int channel, const float* buffer, int size, int idx);

	/** Read incoming data */
	int readData();

	int selectedChannel;

	ScopedPointer<StreamingSocket> connection;

private:

	bool sentHeader;
	int bufferSize;
	ScopedPointer<StreamingSocket> socket;
	int port;
	bool connected; //true if client (Matlab) has connected
	char writeBuffer[WRITE_BUFFER_SIZE];
	char readBuffer[READ_BUFFER_SIZE];
};