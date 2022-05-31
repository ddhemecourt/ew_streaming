function data_out = pdw_constructor_from_table_extended_2(dat,i)

[TOA_, VC_, Word_type_, Type_, Freq_offset_, Level_offset_, ... 
    Phase_offset_, Arb_seg_TON_, Trise_, Tfall_, Edge_shape_, ...
    Repetitions_, Bandwidth_, Barker_code_,Marker1_,Marker2_,Marker3_,Path_, CMD_, Freq_, Level_] = dat{i,:};


if (strcmp(Word_type_,'PDW'))
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Header Section%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
TOA = dec2bin((TOA_*2.4e9),52); %Time of arrival = time(sec)*2.4e9
if(strcmp(Type_,'ARB'))   %SEG --> ARB = '0', real-time signal = '1'
    SEG = '1';
else
    SEG = '0';
end
USE_EXTENSION = '1';    %Use extension
PARAMS = '00';          %Use basic edge shaping turned on
% RSVD_0 = '000' ;      %uncomment when not using extension 
HEADER = strcat(TOA,SEG,USE_EXTENSION,PARAMS);
HEADER_LENGTH = length(HEADER);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Flags Section %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

CTRL = '0'; % Inticates PDW, '1' = TCDW

RSVD_1 = '0'; % Reserved for future use
PHASE_MOD = '0'; %Indicates phase mode 
                 %0 = value in PHS field inside PDW body is absolute
                 %1 = value in PHS field inside PDW body is relative to
                 %the phase value of the last sample of the previous
                 %signal
IGNORE_PDW = '0'; %PDW is ignored (no signal output) 
M4 = '0';         %reserved for future use
M3 = dec2bin(Marker3_);         %Set Marker 3
M2 = dec2bin(Marker2_);        %Set Marker 2
M1 = dec2bin(Marker1_);        %Set Marker 1
                  %Note need to turn on markers in GUI/Programmatically
FLAGS = strcat(CTRL,RSVD_1,PHASE_MOD,IGNORE_PDW,M4,M3,M2,M1);
FLAGS_LENGTH = length(FLAGS);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Body%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FREQ_OFFSET = dec2bin(Freq_offset_/2.4e9*(2^32),32);
LEVEL_OFFSET = dec2bin(10^(-Level_offset_/20)*(2^15-1),16);
PHASE_OFFSET = dec2bin(Phase_offset_/360*2^16,16);

BODY = strcat(FREQ_OFFSET,LEVEL_OFFSET,PHASE_OFFSET);
BODY_LENGTH = length(BODY);
%Params (only in expert mode)%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if (strcmp(Edge_shape_,'LIN'))
	EDGE_TYPE = '000';	%Linear
else
	EDGE_TYPE = '001';	%Cosine
end
multiplier_ = 1;		%hard coded multipler to x1. Could set to 8
MULTIPLIER = '0';		%corresponds to multiplier x1, x8 = '1'
RSVD_10 = '000000';
RISE_FALL_TIME = dec2bin((Trise_ + Tfall_)*10^-9/multiplier_*2.4e9,22); %added time to width 
PARAMS_FIELD = strcat(EDGE_TYPE,MULTIPLIER,RSVD_10,RISE_FALL_TIME);
PARAMS_LENGTH = length(PARAMS_FIELD);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%Payload%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%ARB Segment [SEG = 1]
if(strcmp(Type_,'ARB'))
SEGMENT_IDX = dec2bin(Arb_seg_TON_,24);  %Index of preloaded waveform index
RSVD_3 = dec2bin(0,72);
PAYLOAD = strcat(SEGMENT_IDX,RSVD_3);

elseif(strcmp(Type_,'UNMOD'))
%Real-Time Signal Unmod [SEG = 0, MOD = 0]
MOD0 = '0000';                 %Modulation type 0 unmodulated
TON = dec2bin(Arb_seg_TON_*10^-6*2.4e9,44);   %time on (pulse width)
RSVD_4 = dec2bin(0,48);
PAYLOAD = strcat(MOD0,TON,RSVD_4);

elseif(strcmp(Type_ ,'LCHIRP'))
%Real-Time Signal Linear Chirp [SEG = 0, MOD = 1]
MOD1 = '0001';                 %Modulation type 1 linear chirp
RSVD_5 = dec2bin(0,3);
TON = dec2bin(Arb_seg_TON_*10^-6*2.4e9,25);   %time on (pulse width)
freq_step = (Bandwidth_*10^6)/(bin2dec(TON)-1);   %intermediate calculation from BW
FREQ_INC = dec2bin((freq_step/2.4e9)*(2^64),64); %freq increment calulated
%RSVD_6 = dec2bin(0,24);
PAYLOAD = strcat(MOD1,RSVD_5,TON,FREQ_INC);

elseif(strcmp(Type_,'TCHIRP'))
%Real-Time Signal Triangular Chirp [SEG = 0, MOD = 2]
MOD2 = '0010';                  %Modulation type 2 triangular chirp
RSVD_7 = dec2bin(0,3);
TON = dec2bin(Arb_seg_TON_*10^-6*2.4e9,25);   %time on (pulse width)
freq_step = Bandwidth_*10^6/(bin2dec(TON)-1);   %intermediate calculation from BW
FREQ_INC = dec2bin(freq_step/2.4e9*2^64,64); %freq increment calulated
% RSVD_8 = dec2bin(0,24);
PAYLOAD = strcat(MOD2,RSVD_7,TON,FREQ_INC);

elseif(strcmp(Type_,'BARKER'))
%Real-Time Signal Barker [SEG = 0, MOD = 3]
MOD3 = '0011';
CHIP_WIDTH = dec2bin(Arb_seg_TON_*10^-6*2.4e9,44) %chip width
CODE = dec2bin(Barker_code_,4)             %barker code
RSVD_9 = '0000';
STUFFING_1 = dec2bin(0,16);                         %zero padding
RSVD_10 = dec2bin(0,24);
PAYLOAD = strcat(MOD3,CHIP_WIDTH,CODE,RSVD_9,STUFFING_1,RSVD_10);
end
PAYLOAD_LENGTH = length(PAYLOAD)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Extension Flags%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FIELD_1_TYPE = '001';		%Edge Shaping Field
FIELD_2_TYPE = '000';		%Unused - no burst for now
FIELD_3_TYPE = '000';		%Unused - no burst for now
RSVD_11 = '0000000';

%Field 1 - Edge Shaping

%Edge type and multiplier defined above
RISE_TIME = dec2bin((Trise_*(10^-9)/multiplier_)*2.4e9,22);
FALL_TIME = dec2bin((Tfall_*(10^-9)/multiplier_)*2.4e9,22);
FIELD_1 = strcat(EDGE_TYPE,MULTIPLIER,RISE_TIME,FALL_TIME);
% FIELD_1 = dec2bin(0,48);
%Fields 2 and 3 unused for now
FIELD_2 = dec2bin(0,48);
FIELD_3 = dec2bin(0,48);

EXTENSION_FIELD = strcat(FIELD_1_TYPE,FIELD_2_TYPE,FIELD_3_TYPE,RSVD_11,FIELD_1,FIELD_2,FIELD_3);
EXTENSION_LENGTH = length(EXTENSION_FIELD);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% PDW_PACKET = (strcat(HEADER, FLAGS, BODY, PARAMS_FIELD, PAYLOAD)) %construct packet
PDW_PACKET = (strcat(HEADER, FLAGS, BODY, PAYLOAD, EXTENSION_FIELD)); %construct packet
hexval = dec2hex(bin2dec(PDW_PACKET(1:32)));
LENGTH = length(PDW_PACKET);

data = [];

for n = 0:1:47
    data = [data, uint8(bin2dec(PDW_PACKET(8*n+1:8*(n+1))))];  
end

data_out = data;


elseif(strcmp(Word_type_,'TCDW'))
    %%%%%Header
    TOA = dec2bin((TOA_*2.4e9),52); %Time of arrival = time(sec)*2.4e9
    
    if(strcmp(Path_,'A')), PATH = '0';  %RF Path A or B
    elseif (strcmp(Path_,'B')),PATH = '1'; end
    
    if(strcmp(CMD_,'Freq'))
        CMD = dec2bin(0,3);  %Type of TCDW command
    elseif(strcmp(CMD_,'Amp'))
        CMD = dec2bin(1,3);
    elseif(strcmp(CMD_,'FreqAmp'))
        CMD = dec2bin(2,3);
    elseif(strcmp(CMD_,'Rearm'))
        CMD = dec2bin(3,3); 
    end
    
    HEADER = strcat(TOA, PATH, CMD);
    headerlen = length(HEADER);
    %%%%%Flags
    CTRL = '1';
    RSVD0 = dec2bin(0,7);
    FLAGS = strcat(CTRL, RSVD0);
    flagslen = length(FLAGS);
    %%%%%Body
    FVAL = dec2bin(Freq_, 40);
    %%level calc
    if(Level_<0), bit0 = '1';
    else, bit0 = '0'; end
    dec = dec2bin(abs(Level_),7);
    tens = dec2bin(floor(10*rem(abs(Level_),1)),4);
    hunds = dec2bin(floor(10*rem(10*rem(abs(Level_),1),1)),4);
    zer = dec2bin(0,8);
    
    LVAL = strcat(bit0,dec,tens,hunds,zer);
    
    BODY = strcat(FVAL, LVAL);
    bodylen = length(BODY);
    
    TCDW_PACKET = strcat(HEADER,FLAGS,BODY);
    length(TCDW_PACKET)
    
    data = [];

    for n = 0:1:15
        data = [data, uint8(bin2dec(TCDW_PACKET(8*n+1:8*(n+1))))];  
    end

    data_out = data

end
