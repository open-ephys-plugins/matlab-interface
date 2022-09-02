classdef GenericProcessor < handle

	properties (Access = protected)
		client;
		dataIn;
        dataOut;
        timer;
        dataRate;
        packetsFetched;
	end

	methods

		function self = GenericProcessor(host, port)

			self.client = OEClient(host, port);
			self.dataIn = DataBuffer(self.client);
            self.dataOut = ['Fetch' blanks(self.client.WRITE_MSG_SIZE_IN_BYTES/2-5)];
            self.timer = tic;
            self.dataRate = 0;
            self.packetsFetched = 0;
            
		end

	end

	methods (Access = public)

		function data=process(self)
            self.client.write(self.dataOut);
            data=self.client.read();
            data=char(data);
            self.timer = tic;
		end

		function sendHandshake(self)
			self.client.sendHandshake();
		end

		function startAcquisition(self)
			self.client.startAcquisition();
		end

		function updateChannels(self, channels)
			self.client.updateChannels(channels);
		end

		function stopAcquisition(self)
			self.client.stopAcquisition();
		end

		function disconnect(self)
			self.client.disconnect();
		end

	end

end