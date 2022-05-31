function align_smw(SMW,filename)

    writeread(SMW,":SCONfiguration:RFALignment:STATe 0; *OPC?");
    writeread(SMW,sprintf(":SCONfiguration:RFALignment:SETup:FILE '%s'; *OPC?",strcat("/var/user/",filename,".rfsa")));
    writeread(SMW,":SCONfiguration:RFALignment:STATe 1; *OPC?");
    writeread(SMW,":SCONfiguration:RFALignment:ALIGn; *OPC?");
    
end