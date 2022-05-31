function restart_smw_extended_seq(SMWs)

    if(~isempty(SMWs{1}))
        writeline(SMWs{1},":SOURce1:BB:ESEQuencer:STATe 0");
        writeline(SMWs{1},":SOURce2:BB:ESEQuencer:STATe 0");
    end
    if(~isempty(SMWs{2}))
        writeline(SMWs{2},":SOURce1:BB:ESEQuencer:STATe 0");
        writeline(SMWs{2},":SOURce2:BB:ESEQuencer:STATe 0");
    end
    
    if(~isempty(SMWs{3}))
        writeline(SMWs{3},":SOURce1:BB:ESEQuencer:STATe 0");
        writeline(SMWs{3},":SOURce2:BB:ESEQuencer:STATe 0");
    end
    
    if(~isempty(SMWs{1})), writeread(SMWs{1},"*OPC?"), end
    if(~isempty(SMWs{2})), writeread(SMWs{2},"*OPC?"), end
    if(~isempty(SMWs{3})), writeread(SMWs{3},"*OPC?"), end
    
    if(~isempty(SMWs{1}))
        writeline(SMWs{1},":SOURce1:BB:ESEQuencer:STATe 1")
        writeline(SMWs{1},":SOURce2:BB:ESEQuencer:STATe 1")
    end
    
    if(~isempty(SMWs{2}))
        writeline(SMWs{2},":SOURce1:BB:ESEQuencer:STATe 1")
        writeline(SMWs{2},":SOURce2:BB:ESEQuencer:STATe 1")
    end
    
    if(~isempty(SMWs{3}))
        writeline(SMWs{3},":SOURce1:BB:ESEQuencer:STATe 1")
        writeline(SMWs{3},":SOURce2:BB:ESEQuencer:STATe 1")
    end
    
    if(~isempty(SMWs{1})), writeread(SMWs{1},"*OPC?"), end
    if(~isempty(SMWs{2})), writeread(SMWs{2},"*OPC?"), end
    if(~isempty(SMWs{3})), writeread(SMWs{3},"*OPC?"), end
  
    
end