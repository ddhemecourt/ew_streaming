function streams = pdw_packet_constructor(dat,reps,added_t,BBs)
    VC_idx = 2;                     %alias the VC index from the table
    [M,N] = size(dat);              % get length of the table
    streams = cell(1,BBs);
    for i = 1:BBs
        streams{i} = [];
    end
    for x = 1:reps
        if(x>1)
            for t = 2:M
                dat{t,1} = dat{t,1} + dat{end,1};
            end
        end
        if(added_t > 0)
            for t = 2:M
                dat{t,1} = dat{t,1} + added_t;
            end
        end
        for i = 2:M
                streams{(dat{i,VC_idx})} = [streams{(dat{i,VC_idx})},pdw_constructor_from_table_extended_2(dat,i)];
        end
    end
     
end