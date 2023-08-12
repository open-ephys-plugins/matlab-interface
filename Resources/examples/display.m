host='127.0.0.1'
port=1234
processor=GenericProcessor(host, port);

cycle_duration=0.05 %50ms cycle

timeall=tic();
while 1
    timet=tic();
    data=[];
    t=processor.process();
    if ~isempty(t)
        t=split(t,' ');
        for i=1:length(t)-1
            % For some unknown reason, there are some garbled codes in the data received for the first time
            % If your system language is CHINESE, such codes are presented in the form of '屯'
            % For users of other languages, you may experiment yourself
            % For formal use, simply discard the data obtained the first time
            if ~isempty(t{i}) && t{i}(1)~='屯' 
                tt=split(t{i},'+');
                data=[data;str2double(tt(1)),str2double(tt(2))];
            end
        end
    end
    if ~isempty(data)
        data
    end
    while toc(timet)<cycle_duration
    end
    toc(timeall)
end