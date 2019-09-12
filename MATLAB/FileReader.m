classdef FileReader < handle 
    
    properties
        folderName;
        sampleRate;
        sourceProcessorName;
        sourceProcessorID;
        sourceProcessorSubIdx;
        recordedProcessor;
        recordedProcessorID;
        nChannels;
        channels;
    end
    
    properties
        rawData;
        nSamplesPerChannel;
        channelData;
    end
    
    methods
        
        function self = FileReader(filepath)
            
            fid = fopen(strcat(filepath,'structure.oebin')); 
            raw = fread(fid,inf); 
            str = char(raw'); 
            fclose(fid); 
            json = jsondecode(str);
            
            json.continuous.folder_name; 
            
            t = tic; 
            
            for ii = 1:1%length(json.continuous)
                
                fileID = fopen(strcat(filepath, 'continuous\', json.continuous(ii).folder_name(1:end-1), '\continuous.dat'));
                self.nChannels = json.continuous.num_channels;
                self.rawData = fread(fileID, inf, 'int16');
                [nRawSamples, ~] = size(self.rawData);
                self.nSamplesPerChannel = nRawSamples / self.nChannels;
                self.channelData = zeros(self.nChannels, self.nSamplesPerChannel);
                
                for jj = 2:3 %1:nChannels
                    self.channelData(jj,:) = self.rawData(jj:self.nChannels:end);
                end
                
                self.plotData;
                
            end

            toc(t)
          
        end
        
        function plotData(self)
            
            f = figure;
            
            for ii = 1:4
                plot(self.channelData(ii,1:1000)); hold on;
            end
            
        end
        
    end
    
end






