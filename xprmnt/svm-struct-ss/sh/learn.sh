#!/usr/sh
timestamp="$(date +'%Y%m%d.%H%M%S')"

#
xprmnt_root_dir=/home/tor/xprmnt/svm-struct-ss
train_data_list_filepath=$xprmnt_root_dir/meta/split_voc2010_philipp/Test_dummy.txt

#model_out_dir=$xprmnt_root_dir/ssvm-ss.model.$timestamp
model_out_dir=$xprmnt_root_dir/ssvm-ss.model.test
mkdir -p $model_out_dir

#C: trade-off between training error and margin (default 0.01
C=0.1

# Optimization Options ---
# choice of structural learning algorithm:
# 0: n-slack algorithm described in [1]
# 1: n-slack algorithm with shrinking heuristic
# 2: 1-slack algorithm (primal) described in [5]
# 3: 1-slack algorithm (dual) described in [5]
# 4: 1-slack algorithm (dual) with constraint cache [5]
# 9: custom algorithm in svm_struct_learn_custom.c
w=2

#epsilon: allow that tolerance for termination criterion
e=100

#
learner_exe=../../../svm-struct-ss/build/svm_struct_ss_learn
$learner_exe \
            -c $C \
            -w $w -e $e \
            $train_data_list_filepath \
            $model_out_dir
