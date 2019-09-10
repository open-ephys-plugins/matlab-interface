classdef GenericProcessor < handle

	properties (Access = protected)
		client;
		data;
	end

	methods

		function self = GenericProcessor(host, port)

			self.client = OEClient(host, port);
			self.data 	= DataBuffer(self.client);
            
		end

	end

	methods (Access = protected)

		function process(self)
			%Fetch the raw data and organize it into the DataBuffer
            self.client.write('Test!');
			self.data.continuous = str2num(self.client.read()); %#ok<*ST2NM>
			self.data.numSamplesFetched = length(self.data.continuous);
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