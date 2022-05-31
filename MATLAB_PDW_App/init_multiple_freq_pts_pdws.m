writeline(ZNA, 'SENSe1:SWEep:TYPE CW');
writeline(ZNA, 'SWEep:TIME 0.055');
writeline(ZNA, 'TRIG:SOURce EXT');
writeline(ZNA, 'TRIG:EINP EXTB');
writeline(ZNA, 'INITiate:CONT ON');
% writeline(ZNA, 'CALC:PAR:DEL "Trc2"')
% writeline(ZNA, 'CALC:PAR:DEL "Trc4"')
% writeline(ZNA, 'CALC:PAR:DEL "Trc6"')
% 
% writeline(ZNA, 'CONF:TRACe:REName "Trc3", "Trc2"')
% writeline(ZNA, 'CONF:TRACe:REName "Trc5", "Trc3"')

writeline(ZNA, 'DISP:TRACe1:Y:PDIV 5')
writeline(ZNA, 'DISP:TRACe3:Y:PDIV 5')
writeline(ZNA, 'DISP:TRACe5:Y:PDIV 5')
writeline(ZNA, 'DISP:TRACe1:Y:RLEVel 0')
writeline(ZNA, 'DISP:TRACe3:Y:RLEVel 0')
writeline(ZNA, 'DISP:TRACe5:Y:RLEVel 0')

writeline(ZNA, 'CALC:MARK ON')
writeline(ZNA, 'CALC:MARK:COUPled ON')