#!/usr/sh
timestamp="$(date +'%Y%m%d.%H%M%S')"

#
xprmnt_root_dir=/home/tor/xprmnt/svm-struct-ss
train_data_list_filepath=$xprmnt_root_dir/meta/split_voc2010_philipp/Test_dummy.txt

#model_out_dir=$xprmnt_root_dir/ssvm-ss.model.$timestamp
model_out_dir=$xprmnt_root_dir/ssvm-ss.model.test
mkdir -p $model_out_dir

#
C=0.1

#
learner_exe=../../../svm-struct-ss/build/svm_struct_ss_learn
$learner_exe \
            -c $C \
            $train_data_list_filepath \
            $model_out_dir
