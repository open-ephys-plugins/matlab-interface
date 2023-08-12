#include <JuceHeader.h>
#include "../../Source/Processors/Events/Spike.h"

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

	/** MY CODE*/
	int writeSpike(int channels[], uint16 ids[], int64 sampleIdxs[], int nSpikes);
	int isReady;
	//int writeSpike(int * channels, uint16 * ids);
	//int writeSpike(const SpikeChannel* channel, const Spike* spike);

	//int writeSpike(int electrodeIndex, const Spike * spike);

	int selectedChannel;

	ScopedPointer<StreamingSocket> connection;

	HeapBlock<float> m_scaledBuffer;
	HeapBlock<int16> m_intBuffer;


private:

	bool sentHeader;
	int bufferSize;
	ScopedPointer<StreamingSocket> socket;
	int port;
	bool connected; //true if client (Matlab) has connected
	char writeBuffer[WRITE_BUFFER_SIZE];
	char readBuffer[READ_BUFFER_SIZE];
	int writeBufferPointer=0;
	char t_writeBuffer[WRITE_BUFFER_SIZE] = {0};
};