classdef OEClient < handle
    
    %OECLIENT A client socket for receiving data streams from the Open-Ephys GUI. 
    %   This class encapsulates a socket on a dedicated, local port in order to
    %   receive continous data stream from Open Ephys. 
    
    properties (Access = private)
        host;
        port;
        connected;
        socket;
        in_stream;
        stream_reader;
        buffered_reader;
        eom;
    end
    
    methods
        
        function self = OEClient(host, port)
            
            self.eom = "~";
            
            self.host = host;
            self.port = port;
            
            self.connected = 0;
            
            while ~self.connected 
                self.connect(host,port);
            end
            
            fprintf("Successfully connected!\n");
            
            %Define plot
            plotTitle = 'Open Ephys Data Stream';
            xLabel = 'Sample Count';
            yLabel = 'Voltage [V]';
            x_axis_size = 30000;
            
            clf; cla; plotGraph = plot(0,0); drawnow; hold on;
            title(plotTitle, 'FontSize', 16);
            xlabel(xLabel, 'FontSize', 12);
            ylabel(yLabel, 'FontSize', 12);
            xlim([0,x_axis_size]); ylim([-50, 50]);

            inputLine = self.buffered_reader.readLine; %waits for data stream
         
            t = tic;
            
            lastIdx = 0;
         
            while ishandle(plotGraph)
                
                %Get data from socket
                inputLine = str2num(self.buffered_reader.readLine);
                numSamples = length(inputLine);
                
                %Compute some basic stats
                minV = min(inputLine);
                maxV = max(inputLine);
                avgV = mean(inputLine);
                
                fprintf("Min: %1.2f Max: %1.2f Avg: %1.2f\n", minV, maxV, avgV);

                %Pre-process for plotting
                if (lastIdx + numSamples) > x_axis_size
                    xData = lastIdx:x_axis_size;
                    yData = inputLine(1:(x_axis_size - lastIdx));
                else
                    xData = (lastIdx+1):(lastIdx+numSamples);
                    yData = inputLine;
                end

                %Plot
                try 
                    plotGraph = plot(xData, yData); drawnow; 
                    set(plotGraph, 'XData', xData, 'YData', yData);
                catch
                    fprintf("xDataSize: %d, yDataSize: %d\n", length(xData), length(yData));
                end
                
                %Reset if needed
                lastIdx = lastIdx + numSamples;
                if lastIdx > x_axis_size
                    fprintf("Resetting: lastIdx: %d\n", lastIdx);
                    lastIdx = 0;
                    cla; plotGraph = plot(0,0); drawnow; 
                end
                
            end
            
            fprintf("Elapsed time: %d\n" , int8(toc(t)));
            
        end
        
        function self = connect(self, host, port)
            
            import java.net.Socket
            import java.util.Scanner
            import java.io.*
            
            try
                self.socket = Socket(host, port);
            catch 
                fprintf("Connecting...\n");
            end
            
            fprintf("Created a new socket on %s:%d\n", self.host, self.port);
            self.connected = 1;
            self.in_stream = self.socket.getInputStream;
            fprintf("Created input stream...\n");
            self.stream_reader = InputStreamReader(self.in_stream);
            fprintf("Created input stream reader...\n");
            self.buffered_reader = BufferedReader(self.stream_reader);
            fprintf("Created buffered reader...\n");
           
        end
        
    end
    
end

