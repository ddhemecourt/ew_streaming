function basebands = parse_options(SMW)

    str = writeread(SMW,'*OPT?');
    opts = split(str,',');
    B9s = 0;
    B15s = 0;
    chans = 0;
    for i = 1:length(opts)
        opt = opts(i);
        if(strcmp(opt,"SMW-B9"))
            B9s = B9s + 1;
        elseif(strcmp(opt,"SMW-B15"))
            B15s = B15s + 1;
        end
    end

    basebands = B9s+B15s;
    
end