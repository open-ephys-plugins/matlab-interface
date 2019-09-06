classdef DataBuffer < handle

	properties
		sample_rate;
		continous;
		event;
	end

	properties (Constant)
		MAX_SAMPLES_PER_BUFFER_FRAME = 4096; %TODO: experimentally determine this...
	end

	methods

		function self = DataBuffer(nContinousChannels, nEventChannels)

			continous = containers.Map;
			event = containers.Map;

		end

	end

end