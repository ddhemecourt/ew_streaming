close all

cels1 = {'B2', 'E2', 'I2','H2', 'J2', 'L2', 'N2', 'P2'};
cels2 = {'C2', 'E2', 'G2', 'I2', 'K2', 'M2', 'O2', 'Q2'};
% freq = [4E9 6E9 8E9 10E9 12E9 14E9 16E9 18E9];
freq = [10E9];

% 
% writeline(ZNA, 'SENSe1:SWEep:TYPE CW');
% writeline(ZNA, 'SWEep:TIME 0.055');
% writeline(ZNA, 'TRIG:SOURce EXT');
% writeline(ZNA, 'TRIG:EINP EXTB');
% writeline(ZNA, 'INITiate:CONT ON');
% writeline(ZNA, 'CALC:PAR:DEL "Trc2"')
% writeline(ZNA, 'CALC:PAR:DEL "Trc4"')
% writeline(ZNA, 'CALC:PAR:DEL "Trc6"')
% 
% writeline(ZNA, 'CONF:TRACe:REName "Trc3", "Trc2"')
% writeline(ZNA, 'CONF:TRACe:REName "Trc5", "Trc3"')
% 
% writeline(ZNA, 'DISP:TRACe1:Y:PDIV 5')
% writeline(ZNA, 'DISP:TRACe3:Y:PDIV 5')
% writeline(ZNA, 'DISP:TRACe5:Y:PDIV 5')
% writeline(ZNA, 'DISP:TRACe1:Y:RLEVel 0')
% writeline(ZNA, 'DISP:TRACe3:Y:RLEVel 0')
% writeline(ZNA, 'DISP:TRACe5:Y:RLEVel 0')
% 
% writeline(ZNA, 'CALC:MARK ON')
% writeline(ZNA, 'CALC:MARK:COUPled ON')


for x = 1:1

phase_ch1_2 = [];
phase_ch1_3 = [];
phase_ch1_4 = [];
amp_ch1_2 = [];
amp_ch1_3 = [];
amp_ch1_4 = [];


writeline(SMWs{1}, sprintf(':SOURce1:FREQuency:CW %f', freq(x)));

pause
writeline(ZNA, sprintf('FREQ:CW %f', freq(x)-0.96E9)); 
filename = 'pdw_cal_file_2G_m9p6.xlsx';
send_pdw_streams(filename,1,0, bb_stream_IPs, SMWs, [])
pause(2)
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc1'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc1'"));
phase_ch1_2 = [phase_ch1_2, phase_val]

writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
phase_ch1_3 = [phase_ch1_3, phase_val]

writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc3'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc3'"));
phase_ch1_4 = [phase_ch1_4, phase_val]
% writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
% amp_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
% amp = [amp, amp_val]


pause
writeline(ZNA, sprintf('FREQ:CW %f', freq(x)-0.68E9)); 
filename = 'pdw_cal_file_2G_m6p8.xlsx';
send_pdw_streams(filename,1,0, bb_stream_IPs, SMWs, [])
pause(2)
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc1'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc1'"));
phase_ch1_2 = [phase_ch1_2, phase_val]

writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
phase_ch1_3 = [phase_ch1_3, phase_val]

writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc3'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc3'"));
phase_ch1_4 = [phase_ch1_4, phase_val]
% writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
% amp_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
% amp = [amp, amp_val]


pause
writeline(ZNA, sprintf('FREQ:CW %f', freq(x)-0.24E9)); 
filename = 'pdw_cal_file_2G_m2p4.xlsx';
send_pdw_streams(filename,1,0, bb_stream_IPs, SMWs, [])
pause(2)
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc1'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc1'"));
phase_ch1_2 = [phase_ch1_2, phase_val]

writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
phase_ch1_3 = [phase_ch1_3, phase_val]

writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc3'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc3'"));
phase_ch1_4 = [phase_ch1_4, phase_val]
% writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
% amp_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
% amp = [amp, amp_val]



writeline(ZNA, sprintf('FREQ:CW %f', freq(x))); 
filename = 'pdw_cal_file_2G_0.xlsx';
send_pdw_streams(filename,1,0, bb_stream_IPs, SMWs, [])
pause(2)

writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc1'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc1'"));
phase_ch1_2 = [phase_ch1_2, phase_val]

writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
phase_ch1_3 = [phase_ch1_3, phase_val]

writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc3'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc3'"));
phase_ch1_4 = [phase_ch1_4, phase_val]


% writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
% amp_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
% amp = [amp, amp_val]


pause
writeline(ZNA, sprintf('FREQ:CW %f', freq(x)+0.24E9)); 
filename = 'pdw_cal_file_2G_2p4.xlsx';
send_pdw_streams(filename,1,0, bb_stream_IPs, SMWs, [])
pause(2)
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc1'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc1'"));
phase_ch1_2 = [phase_ch1_2, phase_val]

writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
phase_ch1_3 = [phase_ch1_3, phase_val]

writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc3'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc3'"));
phase_ch1_4 = [phase_ch1_4, phase_val]

% writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
% amp_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
% amp = [amp, amp_val]


pause
writeline(ZNA, sprintf('FREQ:CW %f', freq(x)+0.68E9)); 
filename = 'pdw_cal_file_2G_6p8.xlsx';
send_pdw_streams(filename,1,0, bb_stream_IPs, SMWs, [])
pause(2)
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc1'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc1'"));
phase_ch1_2 = [phase_ch1_2, phase_val]

writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
phase_ch1_3 = [phase_ch1_3, phase_val]

writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc3'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc3'"));
phase_ch1_4 = [phase_ch1_4, phase_val]
% writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
% amp_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
% amp = [amp, amp_val]


pause
writeline(ZNA, sprintf('FREQ:CW %f', freq(x)+0.96E9)); 
filename = 'pdw_cal_file_2G_9p6.xlsx';
send_pdw_streams(filename,1,0, bb_stream_IPs, SMWs, [])
pause(2)
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc1'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc1'"));
phase_ch1_2 = [phase_ch1_2, phase_val]

writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
phase_ch1_3 = [phase_ch1_3, phase_val]

writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc3'"));
phase_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc3'"));
phase_ch1_4 = [phase_ch1_4, phase_val]
% writeline(ZNA, sprintf("CALC1:PAR:SEL 'Trc2'"));
% amp_val = str2double(writeread(ZNA, 'CALC:MARK:Y?'));
% amp = [amp, amp_val]



filename = 'pdw_tests3.xlsx';
% 
% writematrix([phase_ch1_2',phase_ch1_3', phase_ch1_4'],filename,'Sheet',... 
%     1,'Range',cels1{x})

writematrix([phase_ch1_2',phase_ch1_3', phase_ch1_4'],filename,'Sheet',... 
    1,'Range','B110')

% writematrix(amp',filename,'Sheet',1,'Range',cels2{x})

freq_pts = [-0.96 -0.68 -0.24 0 0.24 0.68 0.96];

figure
plot(freq_pts, phase_ch1_2)
xlim([-1 1])
ylim([-3 3])
ylabel('Relative Phase (deg.)')
xlabel('Frequency Offset(GHz)')
title('Relative Phase Between Channels')
hold on
plot(freq_pts, phase_ch1_3)
plot(freq_pts, phase_ch1_4)
legend('Ch2/Ch1', 'Ch3/Ch1', 'Ch4/Ch1')
end

