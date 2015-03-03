function ann_ids = run_VOCevalseg_per_annotation()
    util_dir = '/home/tor/lab1231-sun-prj/util/src-m';
    addpath(util_dir);

    VOCcode_dir = '/home/tor/lab1231-sun-prj/external/VOCdevkit/VOCcode';
    addpath(VOCcode_dir);

    eval_dirpath = '/home/tor/robotics/prj/011/xprmnt/philipp-unary-mrf-voc2010/eval';    
    ann_id_list_dirpath = '/home/tor/robotics/prj/011/xprmnt/philipp-unary-mrf-voc2010/meta/split_voc2010_philipp';
    ann_id_list_filename = 'Test';
    ann_id_list_fileextension = '.txt.ori';
    ann_id_list_filepath = strcat(ann_id_list_dirpath,'/',ann_id_list_filename,ann_id_list_fileextension);
    
    % read ann id from the list
    n_line_to_read = -1;%  If N is smaller than zero, textread reads the entire file.
    ann_ids = textread(ann_id_list_filepath, '%s', n_line_to_read);
            
    n_ann_id = size(ann_ids,1);
    for i=1:n_ann_id
        single_ann_id_list_filename = ann_id_list_filename;
        single_ann_id_list_fileextension = '.txt';% must be .txt to follow the original VOCevalseg.m
        single_ann_id_list_filepath = strcat(ann_id_list_dirpath,'/',single_ann_id_list_filename,single_ann_id_list_fileextension);
        
        fileID = fopen(single_ann_id_list_filepath,'w');    
        fprintf(fileID,'%s',ann_ids{i});
        fclose(fileID);
        
        %       
        [class_acc, global_acc] = run_VOCevalseg(ann_id_list_dirpath, single_ann_id_list_filename);        
        delete(single_ann_id_list_filepath);
        
        % Fix the avg class to exclude NAN values        
        avg_class_acc = mean( class_acc(~isnan(class_acc)) );
        
        %
        local_eval_dirpath = strcat(eval_dirpath,'/',ann_ids{i},'-eval');
        if exist(local_eval_dirpath,'dir')
            rmdir(local_eval_dirpath,'s'); 
        end
        mkdir(local_eval_dirpath);
        
        class_acc_filepath = strcat(local_eval_dirpath,'/',ann_ids{i},'.class_acc');
        csvwrite(class_acc_filepath, class_acc);                               
        
        avg_class_acc_filepath = strcat(local_eval_dirpath,'/',ann_ids{i},'.avg_class_acc');
        csvwrite(avg_class_acc_filepath, avg_class_acc);                               
        
        global_acc_filepath = strcat(local_eval_dirpath,'/',ann_ids{i},'.global_acc');
        csvwrite(global_acc_filepath, global_acc);                               
    end
               
       
end