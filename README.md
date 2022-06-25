# Matlab Interface

![matlab-interface-screenshot](Resources/matlab-interface.png)

Streams one channel of continuous data from the Open Ephys GUI to a live Matlab session. A Matlab API  allows seamless processing of the incoming data in real time.


## Installation

(Coming soon)


## Usage

Once installed, the plugin will appear as 'Matlab Interface' in the 'Filters' section of the 'Processor List' of the Open Ephys GUI. The plugin can be placed after any source, filter, or sink in the signal chain and will stream the data output by the preceding processor into your Matlab session. 

Before starting data acquisition, you will need to press the 'Connect' button in the editor to initiate a connection between Open Ephys and your Matlab session. The plugin will open a socket using the IP and Port address listed in the Matlab Interface editor.

To initialize the connection on the Matlab side, you will need to call your generated script from Matlab following the instructions below. Once the connection has been initiated on both sides, pressing the Play button in the Open Ephys GUI will automatically stream the incoming data to Matlab. 

### Matlab API

In order to process the incoming data in Matlab, you will need to leverage the included Matlab API. The API was designed specifically to introduce/reinforce object oriented programming and to encourage Matlab evangelists to consider developing plugins in C++. 

The Matlab API is centered around a GenericProcessor class that encapsulates an Open Ephys data processor. The idea is to write your own class that inherits and extends a Generic Processor. A starting template is included below: 

```matlab
classdef MyClass < GenericProcessor

    properties
        %define any variables you want to keep track of here (see examples)
    end

    methods 
        function self = Plotter(host, port)
            self = self@GenericProcessor(host, port);
            %Initialize any variables here (see examples)
            self.process();
        end
    end

    methods (Access = protected)
        function process(self)
            while (true) 
                process@GenericProcessor(self); 
                numSamples = self.dataIn.numSamplesFetched;
                data = self.dataIn.continuous(1:end);
                %Do whatever you want with the data here (see examples)
            end
        end
    end
end
```

Here's an example of a simple peak detection algorithm:

```matlab
classdef PeakDetector < GenericProcessor

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
            while (true)
                process@GenericProcessor(self); 
                yd = diff(self.dataIn.continuous)./diff(1:self.dataIn.numSamplesFetched);
                self.peaks = find(~yd);
            end
        end
    end
end
```

And a simple threshold detector... 

```matlab
classdef ThresholdDetector < GenericProcessor

    properties
        thresholdValue;
    end

    methods 
        function self = ThresholdDetector(host, port)
            self = self@GenericProcessor(host, port);
            self.thresholdValue = 3.2; %V
            self.process();
        end
    end

    methods (Access = protected)
        function process(self)
            while (true)
                process@GenericProcessor(self); 
                k = find(self.dataIn.continuous > self.thresholdValue);
            end
        end
    end
end
```

And here's an example of a Plotter that plots the incoming data to a figure in real-time. 

```matlab
classdef Plotter < GenericProcessor

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
            
            self.xAxisRange = [0,80000];
            self.yAxisRange = [-1000 1000];
            
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
            end
        end
    end
end
```

Once you have designed your class, you can call it from the Matlab command window using `MyClass(host,port)`, where host and port need to match the entries specified in the MatlabEngine editor in the OpenEphys GUI. 


## Building from source

First, follow the instructions on [this page](https://open-ephys.github.io/gui-docs/Developer-Guide/Compiling-the-GUI.html) to build the Open Ephys GUI.

**Important:** This plugin is intended for use with the pre-release core application, version 0.6.0. The GUI should be compiled from the [`development-juce6`](https://github.com/open-ephys/plugin-gui/tree/development-juce6) branch, rather than the `master` branch.

Then, clone this repository into a directory at the same level as the `plugin-GUI`, e.g.:
 
```
Code
├── plugin-GUI
│   ├── Build
│   ├── Source
│   └── ...
├── OEPlugins
│   └── matlab-interface
│       ├── Build
│       ├── Source
│       └── ...
```

### Windows

**Requirements:** [Visual Studio](https://visualstudio.microsoft.com/) and [CMake](https://cmake.org/install/)

From the `Build` directory, enter:

```bash
cmake -G "Visual Studio 17 2022" -A x64 ..
```

Next, launch Visual Studio and open the `OE_PLUGIN_matlab-interface.sln` file that was just created. Select the appropriate configuration (Debug/Release) and build the solution.

Selecting the `INSTALL` project and manually building it will copy the `.dll` and any other required files into the GUI's `plugins` directory. The next time you launch the GUI from Visual Studio, the Matlab Interface plugin should be available.


### Linux

**Requirements:** [CMake](https://cmake.org/install/)

From the `Build` directory, enter:

```bash
cmake -G "Unix Makefiles" ..
cd Debug
make -j
make install
```

This will build the plugin and copy the `.so` file into the GUI's `plugins` directory. The next time you launch the GUI compiled version of the GUI, the Matlab Interface plugin should be available.


### macOS

**Requirements:** [Xcode](https://developer.apple.com/xcode/) and [CMake](https://cmake.org/install/)

From the `Build` directory, enter:

```bash
cmake -G "Xcode" ..
```

Next, launch Xcode and open the `matlab-interface.xcodeproj` file that now lives in the “Build” directory.

Running the `ALL_BUILD` scheme will compile the plugin; running the `INSTALL` scheme will install the `.bundle` file to `/Users/<username>/Library/Application Support/open-ephys/plugins-api`. The Matlab Interface plugin should be available the next time you launch the GUI from Xcode.

### Questions and Troubleshooting

If you have any questions and/or issues regarding this plugin, please reach out to pavel@open-ephys.org.
