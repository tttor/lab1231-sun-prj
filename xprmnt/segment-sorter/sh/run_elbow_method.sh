#!/bin/bash
#
run_id=test10e
root_dir=/home/tor/sun4/xprmnt/segment-sorter/kmeans-model
dataset=msrc21

# Determine a list of n_cluster
# >>> K = ''.join([str(int(i))+' ' for i in list(numpy.linspace(50,200,10))])
#K='500 821 1142 1464 1785 2107 2428 2750 3071 3392 3714 4035 4357 4678 5000 '
K='500 1500 '

#
py_ws_dir=/home/tor/sun/ws/lab1231-sun-prj/segment-sorter/topic-discovery
input_dir=/home/tor/sun4/xprmnt/segment-sorter/kmeans-model/$dataset

input_filename=kmeans.20141211.172830.input
input_filepath=$input_dir/$input_filename
used_sample_ratio=1.0

out_dir=$root_dir/$dataset/elbow-method.$run_id/meandistortion
rm -rf $out_dir
mkdir -p $out_dir

#
tmux start-server
for k in $K
do
	session=elbow$k
	tmux kill-session -t $session
	tmux new-session -d -s $session
	tmux select-pane -t $session # Make sure we are in the tumx session

	cmd=python\ $py_ws_dir/run_kmeans.py\ $k\ $used_sample_ratio\ $input_filepath\ $out_dir
	tmux send-keys "$cmd" C-m
done
