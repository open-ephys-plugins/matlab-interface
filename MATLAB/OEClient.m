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
            
            time = 0;
            data = 0;
            count = 0;
            
            inputLineSize = 100;
            
            clf; cla; drawnow; 
            plotGraph = plot(time, data);
            hold on;
            title(plotTitle, 'FontSize', 16);
            xlabel(xLabel, 'FontSize', 12);
            ylabel(yLabel, 'FontSize', 12);
            xlim([0,100*inputLineSize]); ylim([0, 5]);

            inputLine = self.buffered_reader.readLine; %waits for data stream
         
            t = tic;
         
            while ishandle(plotGraph)
                
                %time(count) = count;
                %data(count) = str2double(self.buffered_reader.readLine);
                %set(plotGraph, 'Xdata', time, 'YData', data);
                %axis([0 time(count) yMin yMax]);
                %pause(delay);
                %fprintf("%d: %f\n", count, str2double(inputLine));
                inputLine = str2num(self.buffered_reader.readLine);
                %data(count) = str2double(inputLine);
                if (length(inputLine) == inputLineSize)
                    plotGraph = plot((1+inputLineSize*count):(inputLineSize*(count+1)), inputLine); drawnow; 
                    set(plotGraph, 'XData', (1+inputLineSize*count):(inputLineSize*(count+1)), 'YData', inputLine);
                    count = count + 1;
                else
                    fprintf("Skipping because input length is: %d\n", length(inputLine));
                end
                if count > inputLineSize
                    count = 0;
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

