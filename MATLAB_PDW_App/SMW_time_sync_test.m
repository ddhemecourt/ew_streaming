% FSW = tcpclient("192.168.58.11",5025, "Timeout",30);
writeline(SMWs{1},":SOURce1:BB:ESEQuencer:TRIGger:EXT:DISable 1; *OPC?"); 
writeline(SMWs{1},":SOURce1:BB:ESEQuencer:TRIGger:ARM:EXECute; *OPC?"); 
pause(2)
u = udpport("IPV4");
data = [uint8(hex2dec('A1')) uint8(hex2dec('04')) uint8(hex2dec('B2'))];


%initial PDW
filename = "test_wpu_em.xlsx";
send_pdw_streams(filename,1,0, bb_stream_IPs, SMWs, [])

write(u, data, "192.168.58.15", 7372);
data_out = read(u,80,"uint8")
HHMMSS = data_out(19:21)
HHMMSS_old = HHMMSS;
while(HHMMSS(3) == HHMMSS_old(3))
    write(u, data, "192.168.58.15", 7372);
    data_out = read(u,80,"uint8");
    HHMMSS = data_out(19:21)
    pause(0.01)
end
%arm SMW trigger

writeline(SMWs{1},":SOURce1:BB:ESEQuencer:TRIGger:EXT:DISable 0");
% writeline(FSW,":INIT:IMM;");
% writeread(FSW,"*OPC?")


T0_HHMMSS = [HHMMSS(1:2) HHMMSS(3)+1]

% pause(3)
%read pulse width from capture
% writeline(FSW,":SENS:TRAC:MEAS:DEF:PULS:SEL 1");
% pwid = str2double(writeread(FSW,":SENS:PULS:TIM:PWID? SEL"))
% tstamp = str2double(writeread(FSW,":SENS:PULS:TIM:TST? SEL"))



%     write(u, data, "192.168.58.13", 7372);
%     data_out = read(u,80,"uint8");
%     HHMMSS = data_out(19:21);
% 
%     delta = HHMMSS(3) - T0_HHMMSS(3)
% 
% if((0.0004 < pwid) && (pwid < 0.0006))
%     filename = "pdw_ts_em2.xlsx";
%     writematrix([delta+1+0.005],filename,'Sheet',1,'Range','A2');
% elseif ((0.00004 < pwid)&& (pwid< 0.00006))
%     filename = "pdw_ts_em3.xlsx";
% elseif((0.000004 < pwid)&& (pwid < 0.000006))
%     filename = "pdw_ts_em4.xlsx";
% end
% 
%     
% send_pdw_streams(filename,1,0, bb_stream_IPs, SMWs, [])
% writeline(FSW,":INIT:IMM;");
% writeread(FSW,"*OPC?")
