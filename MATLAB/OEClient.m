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
    end
    
    methods
        
        function self = OEClient(host, port)
            
            self.host = host;
            self.port = port;
            
            self.connected = 0;
            
            while ~self.connected 
                self.connect(host,port);
            end
            
            t = tic;
            fprintf("Successfully connected!\n");
            
            count = 0;
            
            inputLine = self.buffered_reader.readLine;
            
            while inputLine ~= '~'
                count = count + 1;
                fprintf("%d: %s\n", count, inputLine);
                inputLine = self.buffered_reader.readLine;
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

