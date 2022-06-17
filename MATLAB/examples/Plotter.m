classdef Plotter < GenericProcessor
	% Plotter An example of a sink processor that displays the incoming data in a plot.
	%	This processor will continuously plot the incoming data until the figure is closed. 
	%
	% Plotter Properties
	%	hPlot - The handle to the plot

	properties
		hPlot;
        fig;
    end
    
    properties
        xAxisRange;
        yAxisRange;
    end

    methods 

    	function self = Plotter(host, port)
            
            self = self@GenericProcessor(host, port);
            
            self.xAxisRange = [0,80000];
            self.yAxisRange = [-1000 1000];
            
            plotTitle = 'Open Ephys Data Stream';
            xLabel = 'Sample Count';
            yLabel = 'Voltage [uV]';

            self.fig = gcf;

			clf; cla; 
			self.hPlot = plot(0,0); drawnow; hold on;

			title(plotTitle);
			xlabel(xLabel); ylabel(yLabel);
			xlim(self.xAxisRange); ylim(self.yAxisRange);


			self.process();

		end

    end

	methods (Access = protected)

		function process(self)
            
			lastSample = 0;
            xAxisSize = self.xAxisRange(2);
            
			while 1

				process@GenericProcessor(self); 

				numSamples = self.dataIn.numSamplesFetched;

				if lastSample + numSamples > xAxisSize
					xData = (lastSample+1):xAxisSize;
					yData = self.dataIn.continuous(1:(xAxisSize-lastSample));
				else
					xData = (lastSample+1):(lastSample+numSamples);
					yData = self.dataIn.continuous;
				end

				self.hPlot = plot(xData,yData); drawnow;
				set(self.hPlot, 'XData', xData, 'YData', yData); 

				%TODO: Currently ignores (doesn't plot) samples that overshoot x-axis range
				lastSample = lastSample + numSamples;
                if lastSample > xAxisSize
                    lastSample = 0;
                    cla; self.hPlot = plot(0,0); drawnow; 
                end

                if get(self.fig, 'CurrentCharacter') == 'q'
                    break;
                end

            end

		end

	end

end