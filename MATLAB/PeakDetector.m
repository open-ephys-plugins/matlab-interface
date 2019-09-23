classdef PeakDetector < GenericProcessor
	% PeakDetector An example of a filter processor that detects peaks in incoming data.
	%	This processor will examine the incoming data for peaks and send back indexes to peaks in the incoming data.  
	%
	% PeakFinder Properties
	%	TODO:

	properties
		peaks;
    end
    
    methods 

    	function self = PeakDetector(host, port)
            
            self = self@GenericProcessor(host, port);
			self.process();

		end

    end

	methods (Access = protected)

		function process(self)
            
            while (1)
                process@GenericProcessor(self); 
                
                yd = diff(self.dataIn.continuous)./diff(1:self.dataIn.numSamplesFetched);
                k = find(~yd);
                self.dataOut = num2str(k);
                
            end

		end

	end

end