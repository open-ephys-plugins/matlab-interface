classdef ArduinoDriver < GenericProcessor

	properties
		arduino; 
        timer;
        ttl_pin;
        ratio_threshold;
    end
  
    methods 

    	function self = ArduinoDriver(host, port)
            
            self = self@GenericProcessor(host, port);
            
            self.arduino = arduino('COM4', 'Uno');
            self.ttl_pin = '10';
            self.ttl_on_time = 0.5; 
            self.ratio_threshold = 1;
            
            self.timer = tic;
            
			self.process();

		end

    end

	methods (Access = protected)

		function process(self)
            
            while (true)
                
                process@GenericProcessor(self); 
                data = self.dataIn.continuous;
                
                %Compute delta and theta power here...
                delta_pwr = (sum(data.^2))/length(data);
                theta_pwr = (sum(data.^3))/length(data);
                
                %Check the ratio and send a TTL if above threshold
                if (delta_pwr / theta_pwr) > self.ratio_threshold
                    writeDigitalPin(self.arduino, self.ttl_pin, 1);
                    self.timer = tic;
                elseif toc(self.timer) > self.ttl_on_time
                    %Set TTL low if no threshold and TTL has been high long
                    %enough
                    if toc(self.timer) > self.ttl_on_time
                        writeDigitalPin(self.arduino, self.ttl_pin, 0);
                    end
                end
                
            end

		end

	end

end