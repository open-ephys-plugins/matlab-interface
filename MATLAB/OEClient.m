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
        out_stream;
        stream_writer;
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
            x_axis_size = 5000;
            
            clf; cla; plotGraph = plot(0,0); drawnow; hold on;
            title(plotTitle, 'FontSize', 16);
            xlabel(xLabel, 'FontSize', 12);
            ylabel(yLabel, 'FontSize', 12);
            xlim([0,x_axis_size]); ylim([-50, 50]);

            inputLine = self.buffered_reader.readLine; %waits for data stream
         
            t = tic;
            count = 0;
            lastIdx = 0;
            firstData = 0;
            
            self.stream_writer.writeBytes('Testing\n');
            self.stream_writer.flush;
         
            while ishandle(plotGraph)
                
                count = count + 1;
                
                %Get data from socket
                inputLine = str2num(self.buffered_reader.readLine);
                numSamples = length(inputLine);
                
                %Dynamically size the y-axis to fit the data
                if ~firstData
                    minV = min(inputLine);
                    maxV = max(inputLine);
                    avgV = mean(inputLine);
                    ylim([minV, maxV]);
                    firstData = 1;
                else
                    if min(inputLine) < minV
                       minV = min(inputLine);
                       ylim([minV, maxV]);
                    end
                    if max(inputLine) > maxV
                       maxV = max(inputLine);
                       ylim([minV, maxV]);
                    end
                end
                
                %fprintf("Min: %1.2f Max: %1.2f Avg: %1.2f\n", minV, maxV, avgV);

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
                    %fprintf("xDataSize: %d, yDataSize: %d\n", length(xData), length(yData));
                end
                
                %Reset if needed
                lastIdx = lastIdx + numSamples;
                if lastIdx > x_axis_size
                    fprintf("Resetting: lastIdx: %d\n", lastIdx);
                    lastIdx = 0;
                    cla; plotGraph = plot(0,0); drawnow; 
                end
                
                %Check if any requests were made...
                %fprintf('Writing data to OE...\n');
                self.stream_writer.writeBytes('Test!');
                self.stream_writer.flush;
                
                if mod(count, 250) == 0
                    self.stream_writer.writeBytes('Stop!');
                    self.stream_writer.flush;
                    fprintf("Elapsed time: %1.2f\n" , toc(t));
                end
                
            end
            
            fprintf("Elapsed time: %d\n" , int8(toc(t)));
            
        end

        function [self, data] = fetch(self)

            readLine = self.buffered_reader.readLine;

            %TODO: Parse readLine in a way that we can differentiate between continous and event data...

        end

        function self = write(self, message)

            %TODO: Check if message is a valid char and proper length

            if (length(message) == WRITE_MSG_SIZE)
                self.stream_writer.writeBytes(message);
                self.stream_writer.flush;
            end

        end
        
        function self = connect(self, host, port)
            
            import java.net.Socket
            import java.util.Scanner
            import java.io.*
            
            try
                self.socket = Socket(host, port);
                fprintf("Created a new socket on %s:%d\n", self.host, self.port);
                self.connected = 1;
                self.in_stream = self.socket.getInputStream;
                fprintf("Created input stream...\n");
                self.out_stream = self.socket.getOutputStream;
                fprintf("Created output stream...\n");
                self.stream_writer = DataOutputStream(self.out_stream);
                fprintf("Created output stream writer...\n");
                self.stream_reader = InputStreamReader(self.in_stream);
                fprintf("Created input stream reader...\n");
                self.buffered_reader = BufferedReader(self.stream_reader);
                fprintf("Created buffered reader...\n");
            catch 
                fprintf("Connecting...\n");
            end
           
        end
        
    end
    
end

