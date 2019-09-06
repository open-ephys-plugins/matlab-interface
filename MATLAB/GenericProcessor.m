classdef GenericProcessor < handle

	properties
		oeClient;
		data;
	end

	methods

		function self = GenericProcessor(host, port)

			self.oeClient = OEClient(host, port);
			self.data = DataBuffer();

		end

	end

	methods (Access = protected)

		function process(self)
			self.data = oeClient.fetch();
		end

		function sendHandshake(self)
			self.oeClient.sendHandshake();
		end

		function startAcquisition(self)
			self.oeClient.startAcquisition();
		end

		function stopAcquisition(self)
			self.oeClient.stopAcquisition();
		end

		function disconnect(self)
			self.oeClient.disconnect();
		end

	end

end