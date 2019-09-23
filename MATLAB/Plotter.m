classdef Plotter < GenericProcessor
	% Plotter An example of a sink processor that displays the incoming data in a plot.
	%	This processor will continuously plot the incoming data until the figure is closed. 
	%
	% Plotter Properties
	%	hPlot - The handle to the plot

	properties
		hPlot;
    end
    
    properties
        xAxisRange;
        yAxisRange;
    end

    methods 

    	function self = Plotter(host, port)
            
            self = self@GenericProcessor(host, port);
            
            self.xAxisRange = [0,10000];
            self.yAxisRange = [-1,4];
            
            plotTitle = 'Open Ephys Data Stream';
            xLabel = 'Sample Count';
            yLabel = 'Voltage [uV]';

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
            
			while ishandle(self.hPlot) 

				process@GenericProcessor(self); %This refreshes the data buffer

				numSamples = self.dataIn.numSamplesFetched;
                fprintf("Fetched %d samples\n", numSamples);

				if lastSample + numSamples > xAxisSize
					xData = lastSample:xAxisSize;
					yData = self.dataIn.continuous(1:(xAxisSize-lastSample));
				else
					xData = (lastSample+1):(lastSample+numSamples);
					yData = self.dataIn.continuous;
				end

				try 
					self.hPlot = plot(xData,yData); drawnow;
					set(self.hPlot, 'XData', xData, 'YData', yData); 
                    self.hPlot = plot(xd,yd); drawnow;
                    for ii = 1:length(k)
                        line([xd(1)+k(ii), xd(1)+k(ii)], self.yAxisRange,'Color',[1 0 0]); 
                    end  
                    set(self.hPlot, 'Xdata', xd, 'YData', yd);
                    fprintf("Length of data: %d:%d\n", length(xData), length(yData));
				catch
					fprintf('X and Y data do not have same length!\n');
				end

				lastSample = lastSample + numSamples;
                if lastSample > xAxisSize
                    fprintf('Resetting plot...\n');
                    lastSample = 0;
                    cla; self.hPlot = plot(0,0); drawnow; 
                end

			end

			fprintf('Plot window was deleted, exiting Plotter::process() method...\n');

		end

	end

end