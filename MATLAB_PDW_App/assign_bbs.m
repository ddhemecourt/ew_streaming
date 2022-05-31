function list_out = assign_bbs(SMWs,subnet)

    [num_SMWs,wid] = size(SMWs);

    IPs = 200:1:250;
    total_bbs = 0;
    list_out = {};
    
    for i = 1:num_SMWs
        if (SMWs{i,2} == 1)
            config_ext_seq_standard(SMWs{i,1},1,strcat(subnet,num2str(IPs(1))));
            total_bbs = total_bbs + 1;
            list_out{length(list_out)+1} = {i,total_bbs,strcat(subnet,num2str(IPs(1))),'RF_A'};
            IPs = IPs(2:end);
        elseif(SMWs{i,2} == 2)
            config_ext_seq_standard(SMWs{i,1},1,strcat(subnet,num2str(IPs(1))));
            config_ext_seq_standard(SMWs{i,1},2,strcat(subnet,num2str(IPs(2))));
            total_bbs = total_bbs + 1;
            list_out{length(list_out)+1} = {i,total_bbs,strcat(subnet,num2str(IPs(1))),'RF_A'};
            total_bbs = total_bbs + 1;
            list_out{length(list_out)+1} = {i,total_bbs,strcat(subnet,num2str(IPs(2))),'RF_B'};
            IPs = IPs(3:end);
        elseif(SMWs{i,2} == 4 && strcmp(SMWs{i,3},'Standard - 2 basebands'))
            config_ext_seq_standard(SMWs{i,1},1,strcat(subnet,num2str(IPs(1))));
            config_ext_seq_standard(SMWs{i,1},2,strcat(subnet,num2str(IPs(2))));
            total_bbs = total_bbs + 1;
            list_out{length(list_out)+1} = {i,total_bbs,strcat(subnet,num2str(IPs(1))),'RF_A'};;
            total_bbs = total_bbs + 1;
            list_out{length(list_out)+1} = {i,total_bbs,strcat(subnet,num2str(IPs(2))),'RF_B'};
            IPs = IPs(3:end);
        elseif(SMWs{i,2} == 4 && strcmp(SMWs{i,3},'Advanced - 4 basebands'))
            config_ext_seq_advanced(SMWs{i,1},4,subnet,IPs);
            total_bbs = total_bbs + 1;
            list_out{length(list_out)+1} = {i,total_bbs,strcat(subnet,num2str(IPs(1))),'RF_A'};
            total_bbs = total_bbs + 1;
            list_out{length(list_out)+1} = {i,total_bbs,strcat(subnet,num2str(IPs(3))),'RF_A'};
            total_bbs = total_bbs + 1;
            list_out{length(list_out)+1} = {i,total_bbs,strcat(subnet,num2str(IPs(2))),'RF_B'};
            total_bbs = total_bbs + 1;
            list_out{length(list_out)+1} = {i,total_bbs,strcat(subnet,num2str(IPs(4))),'RF_B'};
            IPs = IPs(5:end);
        elseif(SMWs{i,2} == 6 && strcmp(SMWs{i,3},'Standard - 2 basebands'))
            config_ext_seq_standard(SMWs{i,1},1,strcat(subnet,num2str(IPs(1))));
            config_ext_seq_standard(SMWs{i,1},2,strcat(subnet,num2str(IPs(2))));
            total_bbs = total_bbs + 1;
            list_out{length(list_out)+1} = {i,total_bbs,strcat(subnet,num2str(IPs(1))),'RF_A'};
            total_bbs = total_bbs + 1;
            list_out{length(list_out)+1} = {i,total_bbs,strcat(subnet,num2str(IPs(2))),'RF_B'};
            IPs = IPs(3:end);
        elseif(SMWs{i,2} == 6 && strcmp(SMWs{i,3},'Advanced - 6 basebands'))
            config_ext_seq_advanced(SMWs{i,1},6,subnet,IPs);
            total_bbs = total_bbs + 1;
            list_out{length(list_out)+1} = {i,total_bbs,strcat(subnet,num2str(IPs(1))),'RF_A'};
            total_bbs = total_bbs + 1;
            list_out{length(list_out)+1} = {i,total_bbs,strcat(subnet,num2str(IPs(3))),'RF_A'};
            total_bbs = total_bbs + 1;
            list_out{length(list_out)+1} = {i,total_bbs,strcat(subnet,num2str(IPs(5))),'RF_A'};
            total_bbs = total_bbs + 1;
            list_out{length(list_out)+1} = {i,total_bbs,strcat(subnet,num2str(IPs(2))),'RF_B'};
            total_bbs = total_bbs + 1;
            list_out{length(list_out)+1} = {i,total_bbs,strcat(subnet,num2str(IPs(4))),'RF_B'};
            total_bbs = total_bbs + 1;
            list_out{length(list_out)+1} = {i,total_bbs,strcat(subnet,num2str(IPs(6))),'RF_B'};
            IPs = IPs(7:end);
        end
    end
    

end