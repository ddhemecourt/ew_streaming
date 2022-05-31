function config_ext_seq_standard(SMW,BB,ip_address)
        if(BB == 1)
            writeread(SMW,":SOURce1:BB:ESEQuencer:MODE RTCI; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:RTCI:PDWFormat EXP; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:TRIGger:OUTPut1:MODE PDW; *OPC?");
            writeread(SMW,":SYSTem:COMMunicate:BB1:NETWork:IPADdress:MODE STAT; *OPC?");
            writeread(SMW,sprintf(":SYSTem:COMMunicate:BB1:NETWork:IPADdress '%s'; *OPC?",ip_address)); 
            writeread(SMW,":SOURce1:BB:ESEQuencer:STATe 1; *OPC?");
        elseif(BB == 2)
            writeread(SMW,":SOURce2:BB:ESEQuencer:MODE RTCI; *OPC?");
            writeread(SMW,":SOURce2:BB:ESEQuencer:RTCI:PDWFormat EXP; *OPC?");
            writeread(SMW,":SOURce2:BB:ESEQuencer:TRIGger:OUTPut1:MODE PDW; *OPC?");
            writeread(SMW,":SYSTem:COMMunicate:BB2:NETWork:IPADdress:MODE STAT; *OPC?");
            writeread(SMW,sprintf(":SYSTem:COMMunicate:BB2:NETWork:IPADdress '%s'; *OPC?",ip_address)); 
            writeread(SMW,":SOURce2:BB:ESEQuencer:STATe 1; *OPC?");
        end
end