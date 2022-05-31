function config_ext_seq_advanced(SMW,BB,subnet,IPs)
        if(BB == 4)
            writeread(SMW,":SCONfiguration:APPLy; *OPC?");
            writeread(SMW,":SCONfiguration:MODE ESEQ; *OPC?");
            writeread(SMW,":SCONfiguration:APPLy; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:SEQuencer2:STATe 1; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:SEQuencer3:STATe 1; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:SEQuencer4:STATe 1; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:MODE RTCI; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:RTCI:PDWFormat EXP; *OPC?");

            writeread(SMW,":SYSTem:COMMunicate:BB1:NETWork:IPADdress:MODE STAT; *OPC?");
            writeread(SMW,sprintf(":SYSTem:COMMunicate:BB1:NETWork:IPADdress '%s'; *OPC?",strcat(subnet,num2str(IPs(1)))));
            writeread(SMW,":SYSTem:COMMunicate:BB2:NETWork:IPADdress:MODE STAT; *OPC?");
            writeread(SMW,sprintf(":SYSTem:COMMunicate:BB2:NETWork:IPADdress '%s'; *OPC?",strcat(subnet,num2str(IPs(2)))));
            writeread(SMW,":SYSTem:COMMunicate:BB3:NETWork:IPADdress:MODE STAT; *OPC?");
            writeread(SMW,sprintf(":SYSTem:COMMunicate:BB3:NETWork:IPADdress '%s'; *OPC?",strcat(subnet,num2str(IPs(3)))));
            writeread(SMW,":SYSTem:COMMunicate:BB4:NETWork:IPADdress:MODE STAT; *OPC?");
            writeread(SMW,sprintf(":SYSTem:COMMunicate:BB4:NETWork:IPADdress '%s'; *OPC?",strcat(subnet,num2str(IPs(4)))));

            writeread(SMW,":SOURce1:BB:ESEQuencer:TRIGger:SEQuencer1:OUTPut1:MODE PDW; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:TRIGger:SEQuencer2:OUTPut1:MODE PDW; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:TRIGger:SEQuencer3:OUTPut1:MODE PDW; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:TRIGger:SEQuencer4:OUTPut1:MODE PDW; *OPC?");

            writeread(SMW,":SOURce1:BB:ESEQuencer:STATe 1; *OPC?");
            
        elseif(BB == 6)
            writeread(SMW,":SCONfiguration:APPLy; *OPC?");
            writeread(SMW,":SCONfiguration:MODE ESEQ; *OPC?");
            writeread(SMW,":SCONfiguration:APPLy; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:SEQuencer2:STATe 1; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:SEQuencer3:STATe 1; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:SEQuencer4:STATe 1; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:SEQuencer5:STATe 1; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:SEQuencer6:STATe 1; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:MODE RTCI; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:RTCI:PDWFormat EXP; *OPC?");

            writeread(SMW,":SYSTem:COMMunicate:BB1:NETWork:IPADdress:MODE STAT; *OPC?");
            writeread(SMW,sprintf(":SYSTem:COMMunicate:BB1:NETWork:IPADdress '%s'; *OPC?",strcat(subnet,num2str(IPs(1)))));
            writeread(SMW,":SYSTem:COMMunicate:BB2:NETWork:IPADdress:MODE STAT; *OPC?");
            writeread(SMW,sprintf(":SYSTem:COMMunicate:BB2:NETWork:IPADdress '%s'; *OPC?",strcat(subnet,num2str(IPs(2)))));
            writeread(SMW,":SYSTem:COMMunicate:BB3:NETWork:IPADdress:MODE STAT; *OPC?");
            writeread(SMW,sprintf(":SYSTem:COMMunicate:BB3:NETWork:IPADdress '%s'; *OPC?",strcat(subnet,num2str(IPs(3)))));
            writeread(SMW,":SYSTem:COMMunicate:BB4:NETWork:IPADdress:MODE STAT; *OPC?");
            writeread(SMW,sprintf(":SYSTem:COMMunicate:BB4:NETWork:IPADdress '%s'; *OPC?",strcat(subnet,num2str(IPs(4)))));
            writeread(SMW,":SYSTem:COMMunicate:BB5:NETWork:IPADdress:MODE STAT; *OPC?");
            writeread(SMW,sprintf(":SYSTem:COMMunicate:BB5:NETWork:IPADdress '%s'; *OPC?",strcat(subnet,num2str(IPs(5)))));
            writeread(SMW,":SYSTem:COMMunicate:BB6:NETWork:IPADdress:MODE STAT; *OPC?");
            writeread(SMW,sprintf(":SYSTem:COMMunicate:BB6:NETWork:IPADdress '%s'; *OPC?",strcat(subnet,num2str(IPs(6)))));

            writeread(SMW,":SOURce1:BB:ESEQuencer:TRIGger:SEQuencer1:OUTPut1:MODE PDW; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:TRIGger:SEQuencer2:OUTPut1:MODE PDW; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:TRIGger:SEQuencer3:OUTPut1:MODE PDW; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:TRIGger:SEQuencer4:OUTPut1:MODE PDW; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:TRIGger:SEQuencer5:OUTPut1:MODE PDW; *OPC?");
            writeread(SMW,":SOURce1:BB:ESEQuencer:TRIGger:SEQuencer6:OUTPut1:MODE PDW; *OPC?");

            writeread(SMW,":SOURce1:BB:ESEQuencer:STATe 1; *OPC?");
        end
end