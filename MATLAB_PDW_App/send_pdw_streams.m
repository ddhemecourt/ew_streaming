function streams = send_pdw_streams(filename,reps, restart, bb_stream_IPs, SMWs, dat_in)
    sys_t = 0;
    if(restart == 1) %use restart to reset the system clock. this will take longer
        restart_smw_extended_seq(SMWs);
    else
        sys_t = str2double(writeread(SMWs{1},":SOURce1:BB:ESEQuencer:RTCI:STR:STIM?"));
    end
    tcp = open_stream_connections(bb_stream_IPs);
    pause(0.1)
    if(isempty(dat_in)) %generate BB PDWs if not provided at input
        dat = read_pdw_file(filename);
        streams = pdw_packet_constructor(dat,reps, sys_t,length(bb_stream_IPs));
    else %only used if streams were pre-generated from pdw_packet_constructor on command line
        streams = pdw_packet_constructor(dat_in,reps, sys_t,length(bb_stream_IPs));
    end

    for i = 1:length(streams)
        if(~isempty(streams{i}) && ~isempty(tcp{i}))
            write(tcp{i},streams{i})
        end
    end

end