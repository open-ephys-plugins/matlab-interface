classdef DataBuffer < handle

	properties
		sampleRate;				%Sample rate of the incoming data
		numSamplesFetched;		%Number of samples acquired from most recent fetch
		continuous;				%CxN array holding C channels of N samples
		event;					%ExN array holding E channels of N binary samples
	end

	properties (Constant)
		MAX_SAMPLES_PER_BUFFER_FRAME = 4096; %TODO: experimentally determine this...
	end

	methods

		function self = DataBuffer(client)

			%Parse client header and setup data structures...
			self.continuous = [];
			self.event = [];

		end

	end

end