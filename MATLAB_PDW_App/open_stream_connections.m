function tcp = open_stream_connections(bb_stream_IPs)
    tcp = {};
    for i = 1:length(bb_stream_IPs)
        if(~isempty(bb_stream_IPs{i})), tcp{i} = tcpclient(bb_stream_IPs{i},49152); end
    end
end