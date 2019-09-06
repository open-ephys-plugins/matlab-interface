classdef Plotter < GenericProcessor

	properties
		hPlot;
	end

	properties
		lastIdx;
		firstData;
	end

	methods

		function self = Plotter(plotTitle, xLabel, yLabel, xLim, yLim)

			self = self@GenericProcessor();

			clf; cla; 
			self.hPlot = plot(0,0); drawnow; hold on;
			title(plotTitle);
			xlabel(xLabel); ylabel(yLabel);
			xlim([0,xLim]); ylim([yLim(1), yLim(2)]);
			
			lastIdx = 0;
			firstData = 0;

			self.process();

		end

		function process(self)

			while ishandle(self.hPlot) %define a condition to keep processing...could simply be while(1)

				process@GenericProcessor(self); %gets data from open-ephys client

			end

		end

	end

end